#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include <Matrix.h>
#include <flex.hpp>
#include <util.hpp>

static inline void Set3(float dest[3], float x, float y, float z) {
  dest[0] = x;
  dest[1] = y;
  dest[2] = z;
}

static inline void Copy3(const float src[3], float dest[3]) {
  dest[0] = src[0];
  dest[1] = src[1];
  dest[2] = src[2];
}

static inline void Zero3(float dest[3]) {
  dest[0] = 0.0F;
  dest[1] = 0.0F;
  dest[2] = 0.0F;
}

// Quadratic (Halley) forward transform following Quadratic_Approximation_Approach.md,
// specialized to a translational (T3) effector.

static float g_stepsPerDegree = 25.0F / 360.0F;

// Borrowed from hangprinter-flex-compensation (cross shaped lower anchors + top anchor)
constexpr float fourAnchors[4][3] = {
    {16.4F, -1610.98F, -131.53F},
    {1314.22F, 1268.14F, -121.28F},
    {-1415.73F, 707.61F, -121.82F},
    {0.0F, 0.0F, 2299.83F},
};

constexpr float fiveAnchors[5][3] = {
    {0.0F, -2000.0F, -120.0F},
    {2000.0F, 0.0F, -120.0F},
    {0.0F, 2000.0F, -120.0F},
    {-2000.0F, 0.0F, -120.0F},
    {0.0F, 0.0F, 2000.0F},
};

static float activeAnchors[HANGPRINTER_MAX_ANCHORS][3] = {
    {fiveAnchors[A_AXIS][0], fiveAnchors[A_AXIS][1], fiveAnchors[A_AXIS][2]},
    {fiveAnchors[B_AXIS][0], fiveAnchors[B_AXIS][1], fiveAnchors[B_AXIS][2]},
    {fiveAnchors[C_AXIS][0], fiveAnchors[C_AXIS][1], fiveAnchors[C_AXIS][2]},
    {fiveAnchors[D_AXIS][0], fiveAnchors[D_AXIS][1], fiveAnchors[D_AXIS][2]},
    {fiveAnchors[I_AXIS][0], fiveAnchors[I_AXIS][1], fiveAnchors[I_AXIS][2]},
    {0.0F, 0.0F, 0.0F},
    {0.0F, 0.0F, 0.0F},
    {0.0F, 0.0F, 0.0F}};
static size_t activeNumAnchors = 5;
static FlexConfig g_flexConfig{};
static bool g_hasConfig{false};
static size_t g_configAnchors{0};

static void setActiveAnchors(const std::vector<std::array<float, 3>> &anchors) {
  for (size_t i{0}; i < HANGPRINTER_MAX_ANCHORS; ++i) {
    Zero3(activeAnchors[i]);
  }
  activeNumAnchors = std::min(anchors.size(), static_cast<size_t>(HANGPRINTER_MAX_ANCHORS));
  for (size_t i{0}; i < activeNumAnchors; ++i) {
    activeAnchors[i][0] = anchors[i][0];
    activeAnchors[i][1] = anchors[i][1];
    activeAnchors[i][2] = anchors[i][2];
  }
}

static bool parseConfigLine(const std::string &line) {
  std::istringstream iss(line);
  std::string tag;
  if (!(iss >> tag) || tag != "CFG") {
    return false;
  }

  size_t count = 0;
  float stepsPerRev = 25.0F;
  int useFlexInt = 1;
  int ignoreGravity = 0;
  int ignorePretension = 0;

  FlexConfig cfg{};
  if (!(iss >> count >> cfg.spoolBuildupFactor >> cfg.springKPerUnitLength >> cfg.moverWeightKg >> cfg.spoolGearTeeth >>
        cfg.motorGearTeeth >> stepsPerRev >> useFlexInt >> ignoreGravity >> ignorePretension >> cfg.lambdaReg >>
        cfg.tol >> cfg.maxItersTarget >> cfg.gravity)) {
    return false;
  }

  cfg.ignoreGravity = ignoreGravity != 0;
  cfg.ignorePretension = ignorePretension != 0;
  cfg.stepsPerRevolution = stepsPerRev;
  cfg.providedAnchors = count;

  auto loadArray = [&](std::array<float, HANGPRINTER_MAX_ANCHORS> &dest, bool *hasExplicit) -> bool {
    size_t const limit = std::min(count, static_cast<size_t>(HANGPRINTER_MAX_ANCHORS));
    float last = 0.0F;
    bool explicitValues = true;
    for (size_t i{0}; i < limit; ++i) {
      float val = 0.0F;
      if (!(iss >> val)) {
        return false;
      }
      if (std::isnan(val)) {
        val = 0.0F;
        explicitValues = false;
      }
      dest[i] = val;
      last = val;
    }
    if (limit > 0) {
      for (size_t i{limit}; i < HANGPRINTER_MAX_ANCHORS; ++i) {
        dest[i] = last;
      }
    }
    if (hasExplicit) {
      *hasExplicit = explicitValues;
    }
    return true;
  };

  bool hasGuyWireLengths = true;
  if (!loadArray(cfg.spoolRadii, nullptr) || !loadArray(cfg.mechanicalAdvantage, nullptr) || !loadArray(cfg.linesPerSpool, nullptr) ||
      !loadArray(cfg.minPlannedForce, nullptr) || !loadArray(cfg.maxPlannedForce, nullptr) || !loadArray(cfg.guyWireLengths, &hasGuyWireLengths)) {
    return false;
  }

  cfg.hasGuyWireLengths = hasGuyWireLengths && count > 0;

  g_configAnchors = count;
  g_flexConfig = cfg;
  g_hasConfig = true;
  g_stepsPerDegree = stepsPerRev / 360.0F;
  return true;
}

static Flex makeFlex(size_t numAnchors) {
  std::array<std::array<float, 3>, HANGPRINTER_MAX_ANCHORS> anchors{};
  for (size_t i{0}; i < HANGPRINTER_MAX_ANCHORS; ++i) {
    anchors[i][0] = 0.0F;
    anchors[i][1] = 0.0F;
    anchors[i][2] = 0.0F;
  }
  size_t const limit = std::min(numAnchors, activeNumAnchors);
  for (size_t i{0}; i < limit; ++i) {
    anchors[i][0] = activeAnchors[i][0];
    anchors[i][1] = activeAnchors[i][1];
    anchors[i][2] = activeAnchors[i][2];
  }
  Flex flex{anchors, numAnchors};
  if (g_hasConfig) {
    g_flexConfig.providedAnchors = g_configAnchors;
    flex.ApplyConfig(g_flexConfig, numAnchors);
  }
  return flex;
}

static std::array<float, HANGPRINTER_MAX_ANCHORS> originLengths(size_t numAnchors) {
  std::array<float, HANGPRINTER_MAX_ANCHORS> lengths{};
  lengths.fill(0.0F);
  size_t const limit = std::min(numAnchors, activeNumAnchors);
  float origin[3] = {0.0F, 0.0F, 0.0F};
  for (size_t i = 0; i < limit; ++i) {
    lengths[i] = hyp3(activeAnchors[i], origin);
  }
  return lengths;
}

struct SolverResult {
  float pos[3] = {0.0F, 0.0F, 0.0F};
  bool converged{false};
  size_t iterations{0};
  float cost{std::numeric_limits<float>::infinity()};
};


float Flex::ResidualsAndDerivatives(
                    const float linePositions[HANGPRINTER_MAX_ANCHORS],
                    float const pos[3],
                    float residuals[HANGPRINTER_MAX_ANCHORS],
                    float jacobian[HANGPRINTER_MAX_ANCHORS][3],
                    float (*hessians)[3][3]) const noexcept {
	float baseImpact[HANGPRINTER_MAX_ANCHORS] = { 0.0F };
	float impactPlus[HANGPRINTER_MAX_ANCHORS][3] = { 0.0F };
	float impactMinus[HANGPRINTER_MAX_ANCHORS][3] = { 0.0F };
	constexpr float impactStep = 1e-3F;
	constexpr bool flexEnabled = true;

	if (flexEnabled) {
		FlexDistances(pos, baseImpact);
		for (size_t axis = 0; axis < 3; ++axis) {
			float shifted[3] = { pos[0], pos[1], pos[2] };
			shifted[axis] += impactStep;
			FlexDistances(shifted, impactPlus[axis]);
			shifted[axis] = pos[axis];
			shifted[axis] -= impactStep;
			FlexDistances(shifted, impactMinus[axis]);
		}
	}

	float cost = 0.0F;
	for (size_t i = 0; i < numAnchors; ++i) {
		float diff[3] = { pos[0] - anchors[i][0], pos[1] - anchors[i][1], pos[2] - anchors[i][2] };
		float distance = norm(diff);
		if (distance < 1e-6F) {
			distance = 1e-6F;
		}
		const float invLen = 1.0F / distance;
		const float invLen3 = invLen * invLen * invLen;

		const float flex = baseImpact[i];
		const float foundLinePos = distance - distancesOrigin[i] + flex;
		residuals[i] = foundLinePos - linePositions[i];

		if (hessians) {
			float (&H)[3][3] = hessians[i];
			for (size_t r = 0; r < 3; ++r) {
				for (size_t c = 0; c < 3; ++c) {
					const float id = (r == c) ? invLen : 0.0F;
					H[r][c] = id - diff[r] * diff[c] * invLen3;
				}
			}
		}

		for (size_t axis = 0; axis < 3; ++axis) {
			const float impactDeriv = (impactPlus[axis][i] - impactMinus[axis][i]) / (2.0F * impactStep);
			jacobian[i][axis] = diff[axis] * invLen + impactDeriv;
		}

		cost += 0.5F * residuals[i] * residuals[i];
	}
	return cost;
}

static bool solveNormalSystem(const float JTJ[3][3], const float rhs[3], float delta[3]) {
	FixedMatrix<float, 3, 4> system{};
	for (size_t r = 0; r < 3; ++r) {
		for (size_t c = 0; c < 3; ++c) {
			system(r, c) = JTJ[r][c];
		}
		system(r, 3) = rhs[r];
	}
	if (!system.GaussJordan(3, 4)) {
		return false;
	}
	delta[0] = system(0, 3);
	delta[1] = system(1, 3);
	delta[2] = system(2, 3);
	return true;
}


void Flex::AccumulateJtJandGrad(
       float const J[HANGPRINTER_MAX_ANCHORS][3],
       float const residuals[HANGPRINTER_MAX_ANCHORS],
       float JTJ[3][3], float grad[3]) const noexcept {
	for (size_t i = 0; i < 3; ++i) {
		grad[i] = 0.0F;
		for (size_t j = 0; j < 3; ++j) {
			JTJ[i][j] = 0.0F;
		}
	}

	for (size_t i = 0; i < numAnchors; ++i) {
		grad[0] += J[i][0] * residuals[i];
		grad[1] += J[i][1] * residuals[i];
		grad[2] += J[i][2] * residuals[i];

		JTJ[0][0] += J[i][0] * J[i][0];
		JTJ[0][1] += J[i][0] * J[i][1];
		JTJ[0][2] += J[i][0] * J[i][2];
		JTJ[1][0] += J[i][1] * J[i][0];
		JTJ[1][1] += J[i][1] * J[i][1];
		JTJ[1][2] += J[i][1] * J[i][2];
		JTJ[2][0] += J[i][2] * J[i][0];
		JTJ[2][1] += J[i][2] * J[i][1];
		JTJ[2][2] += J[i][2] * J[i][2];
	}
}

SolverResult SolveHybrid(const Flex &flex,
                               const float linePositions[HANGPRINTER_MAX_ANCHORS],
                               float initial[3],
                               float eta,
                               float tol,
                               size_t halleyIters,
                               size_t maxIters,
                               size_t numAnchors) noexcept {
	SolverResult result{};
	result.pos[0] = initial[0];
	result.pos[1] = initial[1];
	result.pos[2] = initial[2];
	size_t iter = 0;
	for (; iter < halleyIters && iter < maxIters; ++iter) {
		float residuals[HANGPRINTER_MAX_ANCHORS] = { 0.0F };
		float J[HANGPRINTER_MAX_ANCHORS][3];
		float H[HANGPRINTER_MAX_ANCHORS][3][3];
		result.cost =
		    flex.ResidualsAndDerivatives(linePositions, result.pos, residuals, J, H);

		float JTJ[3][3];
		float grad[3];
		flex.AccumulateJtJandGrad(J, residuals, JTJ, grad);
		JTJ[0][0] += eta;
		JTJ[1][1] += eta;
		JTJ[2][2] += eta;

		float deltaLm[3] = { 0.0F };
		float rhs1[3] = {-grad[0], -grad[1], -grad[2]};
		if (!solveNormalSystem(JTJ, rhs1, deltaLm)) {
			break;
		}

		float Hbar[HANGPRINTER_MAX_ANCHORS][3];
		for (size_t i = 0; i < numAnchors; ++i) {
			Hbar[i][0] = deltaLm[0] * H[i][0][0] + deltaLm[1] * H[i][1][0] + deltaLm[2] * H[i][2][0];
			Hbar[i][1] = deltaLm[0] * H[i][0][1] + deltaLm[1] * H[i][1][1] + deltaLm[2] * H[i][2][1];
			Hbar[i][2] = deltaLm[0] * H[i][0][2] + deltaLm[1] * H[i][1][2] + deltaLm[2] * H[i][2][2];
		}

		float Jbar[HANGPRINTER_MAX_ANCHORS][3];
		for (size_t i = 0; i < numAnchors; ++i) {
			Jbar[i][0] = J[i][0] + 0.5F * Hbar[i][0];
			Jbar[i][1] = J[i][1] + 0.5F * Hbar[i][1];
			Jbar[i][2] = J[i][2] + 0.5F * Hbar[i][2];
		}

		float JTJ2[3][3];
		float grad2[3];
		flex.AccumulateJtJandGrad(Jbar, residuals, JTJ2, grad2);
		JTJ2[0][0] += eta;
		JTJ2[1][1] += eta;
		JTJ2[2][2] += eta;

		float delta[3] = { 0.0F };
		float rhs2[3] = {-grad2[0], -grad2[1], -grad2[2]};
		if (!solveNormalSystem(JTJ2, rhs2, delta)) {
			break;
		}

		result.pos[0] = result.pos[0] + delta[0];
		result.pos[1] = result.pos[1] + delta[1];
		result.pos[2] = result.pos[2] + delta[2];
		result.iterations = iter + 1;
		if (norm(delta) < tol) {
			result.converged = true;
			break;
		}
	}

	for (; iter < maxIters && !result.converged; ++iter) {
		float residuals[HANGPRINTER_MAX_ANCHORS] = { 0.0F };
		float J[HANGPRINTER_MAX_ANCHORS][3];
		result.cost =
		    flex.ResidualsAndDerivatives(linePositions, result.pos, residuals, J, nullptr);

		float JTJ[3][3];
		float grad[3];
		flex.AccumulateJtJandGrad(J, residuals, JTJ, grad);
		JTJ[0][0] += eta;
		JTJ[1][1] += eta;
		JTJ[2][2] += eta;

		float delta[3] = { 0.0F };
		float rhs3[3] = {-grad[0], -grad[1], -grad[2]};
		if (!solveNormalSystem(JTJ, rhs3, delta)) {
			break;
		}

		result.pos[0] = result.pos[0] + delta[0];
		result.pos[1] = result.pos[1] + delta[1];
		result.pos[2] = result.pos[2] + delta[2];
		result.iterations = iter + 1;
		if (norm(delta) < tol) {
			result.converged = true;
			break;
		}
	}

	float residuals[HANGPRINTER_MAX_ANCHORS] = { 0.0F };
	float Jtmp[HANGPRINTER_MAX_ANCHORS][3];
	result.cost =
	    flex.ResidualsAndDerivatives(linePositions, result.pos, residuals, Jtmp, nullptr);
	return result;
}

static SolverResult SolveHalley(const Flex &flex, const std::vector<float> &linePositions, float seed[3],
                                float eta, float tol, size_t maxIters) {
  return SolveHybrid(flex, linePositions.data(), seed, eta, tol, maxIters, maxIters, linePositions.size());
}

static SolverResult solveLm(const std::vector<std::array<float, 3>> & /*anchors*/,
                            const std::vector<float> &linePositions,
                            const std::array<float, HANGPRINTER_MAX_ANCHORS> & /*origins*/,
                            const Flex &flex, float seed[3], float eta, float tol, size_t maxIters) {
  return SolveHybrid(flex, linePositions.data(), seed, eta, tol, 0, maxIters, linePositions.size());
}

static std::vector<float> motorPosToLinePositions(const Flex &flex, const std::array<float, HANGPRINTER_MAX_ANCHORS> &motorPos,
                                                  size_t numAnchors) {
  std::vector<float> linePositions(numAnchors, 0.0F);
  for (size_t i = 0; i < numAnchors; ++i) {
    const float steps = motorPos[i] * g_stepsPerDegree;
    linePositions[i] = flex.MotorPosToLinePos(steps, i);
  }
  return linePositions;
}

#ifndef RUN_DATASET_CLI
int main() {
  constexpr size_t numAnchors = 5;
  Flex const flex = makeFlex(numAnchors);
  auto const origins = originLengths(numAnchors);

  // Dataset from motorstepstocartesiantest.cpp ("With flex max 120 target 20")
  std::vector<std::array<float, HANGPRINTER_MAX_ANCHORS>> motorPoss = {
      {-369.47366890F, -375.18362935F, 318.27641438F, 323.88876891F, 1076.68163402F},
      {-5980.29607895F, 12630.56774248F, 12288.67350302F, -6506.18962476F, -13348.06053879F},
      {-2353.64278337F, 15870.99892165F, 15755.46225500F, -2522.71380922F, -27477.49741579F},
      {1433.28912224F, 10685.68516553F, 1454.31086905F, -10542.99803343F, 1813.47411829F},
      {3112.53568603F, 11779.51141393F, 3231.55807349F, -7609.55533435F, -17364.20990461F},
      {7221.24317502F, 14331.47424095F, 6657.95387927F, -1853.21287105F, -36135.10874008F},
      {10106.78583366F, 10693.29200030F, -7583.66231627F, -8505.26876850F, 6010.84493879F},
      {11680.14400927F, 12421.57492733F, -5209.71780789F, -6326.70127134F, -14780.55685279F},
      {16309.54520647F, 16482.81831770F, -1424.01387322F, -1674.11174073F, -29909.99659944F},
      {-10145.44532568F, 1094.96136691F, 10156.51947369F, 1434.73615456F, 3572.60726310F},
      {-7640.99960502F, 3134.81886277F, 11567.99443696F, 2933.32488052F, -16735.05779004F},
      {-2825.34894707F, 7163.91339055F, 14266.90144144F, 5840.37464568F, -33865.11200386F},
      {-7448.08057312F, 10796.70751776F, 10151.74389637F, -8459.78905687F, 3271.18757879F},
      {2211.41905092F, 2111.20882241F, 858.18022603F, 961.42598207F, -17727.66189890F},
      {6585.49993026F, 5349.29768666F, 5599.61645498F, 6829.22955353F, -40179.35611831F},
      {9827.26906637F, 973.41612678F, -9847.89764725F, 1374.65365736F, 4088.32777321F},
      {11672.37320821F, 3419.76976053F, -7745.30493425F, 2691.62201517F, -16638.39998295F},
      {13421.03929287F, 6834.73758758F, -2414.14469240F, 5523.60605573F, -33907.52745615F},
      {-8153.62357389F, -7386.54442994F, 10370.98726659F, 9875.86705528F, 3214.03835550F},
      {-6347.85480184F, -5493.48884529F, 12625.31736324F, 12063.42984255F, -14568.06462464F},
      {-2585.49492187F, -1676.70121477F, 15851.52670557F, 15223.11520330F, -28367.04177068F},
      {730.23171944F, -9759.54189457F, 1580.93148691F, 9745.76583762F, 3967.14372473F},
      {2429.55959648F, -7123.34974121F, 3539.66195357F, 11159.52316296F, -17372.69974680F},
      {6079.30436810F, -2972.78187516F, 7172.81870678F, 14578.78537233F, -33851.97153806F},
      {11195.81863308F, -7791.69855043F, -8587.42819506F, 10696.72883673F, 3279.60983059F},
      {12162.75651477F, -6552.47720614F, -6177.61954587F, 12406.22670936F, -12538.57963324F},
      {14653.37178962F, -2366.53557957F, -1021.48237225F, 15599.48126071F, -29623.47054342F},
  };

  std::vector<std::array<float, 3>> expectedPoss = {
      {{17.9255679F, -17.6352632F, -27.497151F}},
      {{-523.885148F, -501.061599F, 507.741532F}},
      {{-538.35251F, -530.255594F, 953.040004F}},
      {{-545.189816F, -0.55803779F, 26.4689901F}},
      {{-520.999273F, -3.27341154F, 537.019049F}},
      {{-470.799477F, 16.5498517F, 1047.17018F}},
      {{-505.362595F, 467.529134F, -42.5453256F}},
      {{-513.234032F, 464.122399F, 533.480869F}},
      {{-538.29312F, 526.060302F, 1030.39159F}},
      {{8.98405788F, -520.642383F, -26.6148823F}},
      {{-5.52678502F, -514.840431F, 517.628213F}},
      {{-38.6000122F, -492.617793F, 991.936222F}},
      {{-507.629503F, -465.97604F, 31.7537695F}},
      {{-30.5601452F, 35.9665477F, 458.719776F}},
      {{43.2898928F, 28.8400594F, 1046.72968F}},
      {{10.5874156F, 504.407891F, -44.2588518F}},
      {{-19.9804722F, 520.41744F, 517.004705F}},
      {{-37.9958866F, 454.266612F, 975.222737F}},
      {{455.930483F, -487.688259F, 26.0515823F}},
      {{482.712486F, -520.125685F, 535.481389F}},
      {{495.517745F, -539.35609F, 965.228753F}},
      {{500.096373F, -22.437457F, -42.0139801F}},
      {{490.657689F, -30.4215669F, 527.319817F}},
      {{506.742907F, -31.9689121F, 998.360926F}},
      {{490.590662F, 523.118645F, 41.4092948F}},
      {{517.848381F, 501.639033F, 482.878157F}},
      {{525.399857F, 459.863258F, 981.665044F}},
  };

  float totalErrHalley = 0.0F;
  float totalErrHybrid = 0.0F;
  float totalErrLm = 0.0F;

  std::cout << "idx, halley_err(mm), hybrid_err(mm), lm_err(mm), halley_it, hybrid_it, lm_it\n";
  float seed[3] = {0.0F, 0.0F, 0.0F};
  for (size_t i = 0; i < motorPoss.size(); ++i) {
    auto linePositions = motorPosToLinePositions(flex, motorPoss[i], numAnchors);
    std::vector<std::array<float, 3>> anchorVec;
    anchorVec.reserve(numAnchors);
    for (size_t k = 0; k < numAnchors; ++k) {
      anchorVec.push_back({fiveAnchors[k][0], fiveAnchors[k][1], fiveAnchors[k][2]});
    }

    SolverResult halley =
        SolveHalley(linePositions, seed, 1e-3F, 1e-3F, 20);
    SolverResult hybrid =
        SolveHybrid(flex, linePositions.data(), seed, 1e-3F, 1e-3F, 3, 30, numAnchors);
    SolverResult lm = solveLm(anchorVec, linePositions, origins, flex, seed, 1e-3F, 1e-3F, 30);

    float halleyErr[3] = { 0.0F };
    halleyErr[0] = expectedPoss[i][0] - halley.pos[0];
    halleyErr[1] = expectedPoss[i][1] - halley.pos[1];
    halleyErr[2] = expectedPoss[i][2] - halley.pos[2];
    float hybridErr[3] = { 0.0F };
    hybridErr[0] = expectedPoss[i][0] - hybrid.pos[0];
    hybridErr[1] = expectedPoss[i][1] - hybrid.pos[1];
    hybridErr[2] = expectedPoss[i][2] - hybrid.pos[2];
    float lmErr[3] = { 0.0F };
    lmErr[0] = expectedPoss[i][0] - lm.pos[0];
    lmErr[1] = expectedPoss[i][1] - lm.pos[1];
    lmErr[2] = expectedPoss[i][2] - lm.pos[2];
    float halleyNorm = norm(halleyErr);
    float hybridNorm = norm(hybridErr);
    float lmNorm = norm(lmErr);
    totalErrHalley += halleyNorm;
    totalErrHybrid += hybridNorm;
    totalErrLm += lmNorm;

    std::cout << i << ", " << halleyNorm << ", " << hybridNorm << ", " << lmNorm << ", " << halley.iterations << ", "
              << hybrid.iterations << ", " << lm.iterations << '\n';

    // Warm start from previous pose to mirror controller behaviour
    Copy3(halley.pos, seed);
  }

  const float n = static_cast<float>(motorPoss.size());
  std::cout << "Mean error (Halley): " << (totalErrHalley / n) << " mm\n";
  std::cout << "Mean error (Hybrid Halley->LM): " << (totalErrHybrid / n) << " mm\n";
  std::cout << "Mean error (LM baseline): " << (totalErrLm / n) << " mm\n";
  return 0;
}
#else
int main() {
  std::ios::sync_with_stdio(false);
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    if (line.rfind("CFG", 0) == 0) {
      parseConfigLine(line);
      continue;
    }

    std::istringstream iss(line);
    size_t numAnchors = 0;
    int useFlexInt = 1;
    if (!(iss >> numAnchors >> useFlexInt)) {
      std::cout << "fail 0 0 0 0 0 0\n";
      continue;
    }
    if (numAnchors == 0 || numAnchors > HANGPRINTER_MAX_ANCHORS) {
      std::cout << "fail 0 0 0 0 0 0\n";
      continue;
    }

    std::array<float, HANGPRINTER_MAX_ANCHORS> motorPos{};
    motorPos.fill(0.0F);
    for (size_t i{0}; i < numAnchors; ++i) {
      if (!(iss >> motorPos[i])) {
        std::cout << "fail 0 0 0 0 0 0\n";
        continue;
      }
    }

    std::vector<std::array<float, 3>> anchors(numAnchors);
    for (size_t i{0}; i < numAnchors; ++i) {
      if (!(iss >> anchors[i][0] >> anchors[i][1] >> anchors[i][2])) {
        std::cout << "fail 0 0 0 0 0 0\n";
        continue;
      }
    }
    setActiveAnchors(anchors);

    Flex const flex = makeFlex(numAnchors);
    auto const origins = originLengths(numAnchors);
    auto linePositions = motorPosToLinePositions(flex, motorPos, numAnchors);
    std::vector<std::array<float, 3>> anchorVec = anchors;

    float seed[3] = {0.0F, 0.0F, 0.0F};
    auto const start = std::chrono::steady_clock::now();
    SolverResult hybrid =
        SolveHybrid(flex, linePositions.data(), seed, 1e-3F, 1e-3F, 3, 30, numAnchors);
    auto const end = std::chrono::steady_clock::now();
    double const ms = std::chrono::duration<double, std::milli>(end - start).count();

    std::cout << (hybrid.converged ? "ok" : "fail") << ' ' << hybrid.pos[0] << ' ' << hybrid.pos[1] << ' '
              << hybrid.pos[2] << ' ' << hybrid.iterations << ' ' << hybrid.cost << ' ' << ms << '\n';
  }
  return 0;
}
#endif
