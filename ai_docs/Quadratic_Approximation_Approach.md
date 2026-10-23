![](_page_0_Picture_0.jpeg)

# Fast and Reliable Iterative Cable-Driven Parallel Robot Forward Kinematics: A Quadratic Approximation Approach

Henry Mahnke and Ryan J. Caverly(B[\)](http://orcid.org/0000-0001-7315-7322) 

Department of Aerospace Engineering and Mechanics, University of Minnesota, Minneapolis, MN 55455, USA {mahnk040,rcaverly}@umn.edu

Abstract. This paper presents iterative forward kinematics algorithms for cable-driven parallel robots (CDPRs) that are based on Halley's method. In contrast to other iterative forward kinematics methods that use a linearization of the CDPRs loop-closure equations, such as Newton's method or the Levenberg-Marquardt method, Halley's method uses a second-order Taylor series approximation of these equations. A hybrid method is also proposed that performs a Halley update for the first few iterations and then switches to a Levenberg-Marquardt update. The proposed algorithms are applied to a six degree-of-freedom suspended CDPR and are shown to reduce the number of iterations and increase the rate of successful convergence to the truth pose compared to the Levenberg-Marquardt method. The proposed hybrid method reduces the computation time required for convergence compared to the Levenberg-Marquardt method in the presence of large initial estimation errors.

Keywords: Cable-driven parallel robots · Pose estimation · Forward kinematics · Halley's method

# 1 Introduction

Cable-driven parallel robots (CDPRs) use the actuation of cables to transmit actuating forces to an end-effector or payload. Compared to traditional serial manipulators, CDPRs can feature significantly larger workspaces and payloadto-weight ratios. Forward kinematics is typically used to determine a CDPR's end-effector pose (position and orientation) during operation through measurements of the CDPR's cable lengths obtained from winch encoders. It is critical that the CDPR's pose be estimated accurately and quickly in order to control the end-effector motion precisely, especially during high-acceleration maneuvers.

Forward kinematics algorithms are computed based on the CDPR's loopclosure equations that describe how cable-length measurements relate to the CDPR's geometry. In their simplest form, these loop-closure equations assume straight, inelastic, and massless cables. More complex features can be incorporated into the loop-closure equations, such as cable sag [8], pulley kinematics [17], and cable elasticity [9]. Even the simplest of loop-closure equations are nonlinear functions of the CDPR end-effector's pose, and these nonlinearities typically become more pronounced with more complex models. This highlights one of the challenges of CDPR forward kinematics, which in essence is a nonlinear root solving problem that may have multiple feasible pose solutions. This challenge is exacerbated when significant measurement noise and/or calibration errors are present [3,12]. A common practical method of solving a CDPR's forward kinematics involves linearizing the CDPR's nonlinear loop-closure equations about an initial pose estimate and iteratively solving for an improved estimate using Newton's method or a damped version of this, known as the Levenberg-Marquardt method [2,11–14,17]. Although these approaches can be effective in practice, they can potentially suffer from a large number of iterations and computation time, or not even converge within a reasonable tolerance of the true pose, especially when the initial pose estimation error is relatively large, such as on startup of a CDPR.

Recently, Halley's method, also known as the method of tangent hyperbolas [15, pp. 122–124], [18], was proposed as a faster alternative to Newton's method when solving the inverse kinematics of overactuated serial manipulators [7]. Halley's method uses a second-order Taylor series expansion of the loop-closure equations, rather than the first-order expansion used with Netwon's method. This quadratic approximation of the nonlinearities in the loop-closure equations allows for convergence of an iterative method with fewer iterations and increases the rate of successful convergence for larger initial errors.

This paper adapts the work of [7] to yield novel CDPR iterative forward kinematics algorithms based on Halley's method that are both fast and have a high success rate. The first algorithm proposed is directly based on Halley's method, while the second algorithm is a hybrid method that implements Halley's method for the first few iterations, then switches to the Levenberg-Marquardt method for subsequent iterations. Although the proposed algorithms are adaptable to any form of loop-closure equations that are twice differentiable with respect to the end-effector pose, the preliminary investigation in this paper focuses on an .8-cable .6-degree-of-freedom (DOF) CDPR. Analytic matrix expressions for the majority of the quadratic terms involved in the algorithm are derived, which assist in speeding up the computation time. Confidence intervals on the pose estimates obtained with the proposed algorithms are also computed in the form of covariance matrices leveraging the work of [11,12]. This allows for the methods to be used effectively within a sensor fusion scheme with additional measurements [5,10]. The contributions of this paper are the formulation and numerical evaluation of two novel CDPR iterative forward kinematics algorithms that are faster and have a higher rate of successful convergence compared to the Levenberg-Marquardt method. It is worth noting that unlike the work of [1,8], the algorithms proposed in this paper are not aimed at solving for all possible forward kinematics solutions. Rather, they are aimed at quickly and reliably solving for a local solution within a reasonable neighborhood of an initial pose estimate, with the intention of accounting for relatively large errors on startup.

The remainder of this section presents important notation and a description of the CDPR considered in this paper.

![](_page_2_Figure_1.jpeg)

Fig. 1. A suspended 6-DOF CDPR with a rigid-body end-effector payload.

#### 1.1 Notation and System Description

In this paper, an  $n \times n$  identity matrix is denoted as  $\mathbf{1}\_{n \times n}$  and an  $n \times m$  matrix of zeros is  $\mathbf{0}\_{n \times m}$ . The cross operator,  $(\cdot)^{\times} : \mathbb{R}^3 \to \mathfrak{so}(3)$ , satisfies  $\mathbf{u}^{\times} \mathbf{w} = -\mathbf{w}^{\times} \mathbf{u}$  for all  $\mathbf{u}, \mathbf{w} \in \mathbb{R}^3$ , where  $\mathfrak{so}(3) = {\mathbf{S} \in \mathbb{R}^{3 \times 3} | \mathbf{S} + \mathbf{S}^\mathsf{T} = \mathbf{0}}$  [\[6\]](#page-2-6), pp. 525–526.This work considers an overconstrained 6-DOF CDPR with  $m$  cables, where  

 $m > 6$ . An example of a 6-DOF CDPR with 8 cables is shown in Fig. 1. The  

reference frame  $\mathcal{F}\_o$  is an inertial frame with point  $o$  at its origin, while  $\mathcal{F}\_p$  is an  

end-effector-fixed frame centered at point  $p$ , the end-effector's center of mass.  

The position vector of point  $p$  relative to  $o$  is  $\underline{r}$ , which is resolved in  $\mathcal{F}\_o$  as  

 $\mathbf{r} \in \mathbb{R}^3$ . The position vector of the  $i$ <sup>th</sup> cable's anchoring pulley relative to point  

 $o$  is  $\underline{a}\_i$ , which is resolved in  $\mathcal{F}\_o$  as  $\mathbf{a}\_i$ . The position vector of the  $i$ <sup>th</sup> cable's  

attachment point on the end-effector relative to its anchoring point is  $\underline{l}\_i$ , which  

is resolved in  $\mathcal{F}\_o$  as  $\mathbf{l}\_i$ . The position vector of the  $i$ <sup>th</sup> cable's attachment point  

on the end-effector relative to point  $p$  is  $\underline{b}\_i$ , which is resolved in  $\mathcal{F}\_p$  as  $\mathbf{b}\_i$ . The  

pose of the CDPR's end-effector is described by  $\rho \in \mathbb{R}^6$ , where  $\rho^\text{T} = [\mathbf{r}^\text{T} \,\mathbf{\theta}^\text{T}]$   

and  $\mathbf{\theta} \in \mathbb{R}^3$  represents an unconstrained parameterization of the rotation matrix  

 $\mathbf{R}\_{po}(\mathbf{\theta})$  or direction cosine matrix (DCM)  $(\mathbf{C}\_{po}(\mathbf{\theta}) = \mathbf{R}\_{po}^\text{T}(\mathbf{\theta}))$  that describes the  

orientation of  $\mathcal{F}\_p$  relative to  $\mathcal{F}\_o$ . In this paper a 3-2-1 Euler angle sequence (yaw-  

pitch-roll) is used to parameterize the end-effector orientation, although other  

unconstrained parameterizations could be used, such as those discussed in [\[11\]](#page-2-1).### 2 Forward Kinematics Formulation

This section presents the proposed iterative forward kinematics methods, including the loop-closure and measurement equations, a second-order Taylor series expansion of the measurement equations, and the two proposed algorithms.

### 2.1 Loop-Closure and Measurement Equations

Assuming that the cables are inelastic and pulley kinematics can be ignored, the pose-dependent position vector of the  $i^{th}$  cable's attachment point on the end-effector relative to its attachment is given by the loop-closure equation

$$\ell\_i(\rho) = \mathbf{r} + \mathbf{C}\_{po}^{\top}(\theta)\mathbf{b}\_i - \mathbf{a}\_i \dots$$

It is also assumed that measurements of the CDPR's cable lengths are avail-able through the use of incremental encoders and calibration. Each cable-lengthmeasurement satisfies  $y\_i = ||l\_i(\rho)||\_2 + v\_i$ , where  $y\_i$  is the length measurement ofthe  $i^{th}$  cable and  $v\_i$  represents uncertainty or noise in the cable-length measurement. For the purpose of simplicity in this preliminary study, it is assumed that $v\_i \in N(0, \sigma\_i<sup>2</sup>)$  is zero-mean Gaussian white noise with standard deviation  $\sigma\_i$ .The measurements for all .m cables are concatenated together to form the measurement equation

$$\mathbf{f}(\mathbf{y}, \boldsymbol{\rho}, \mathbf{v}) = \mathbf{0}\_{m \times 1},\tag{1}$$

where  $\mathbf{f}^{\text{T}}(\mathbf{y}, \boldsymbol{\rho}, \mathbf{v}) = [f\_1(y\_1, \boldsymbol{\rho}, v\_1) \cdots f\_m(y\_m, \boldsymbol{\rho}, v\_m)], \mathbf{y}^{\text{T}} = [y\_1 \cdots y\_m], \mathbf{v}^{\text{T}} = [v\_1 \cdots v\_m]$ , and
$$f\_{i}(y\_{i}, \rho, v\_{i}) = -y\_{i} + ||\ell\_{i}(\rho)||\_{2} + v\_{i}.$$

Although cable-length measurement equations are used in this paper, it is
worth noting that the formulation presented in this section can be adapted to
other measurement equations, such as the cable-length-squared equations used
in [\[11\]](#page-3-1), provided that the equations are twice differentiable with respect the
CDPR pose.### 2.2 Taylor Series Expansion

Performing a Taylor series expansion of [\(1\)](#page-3-1) about  $\rho = \bar{\rho}$  and  $\mathbf{v} = \mathbb{E}[\mathbf{v}] = \mathbf{0}\_{m \times 1}$  results in
$$\mathbf{f}(\mathbf{y},\rho,\mathbf{v}) \approx \mathbf{f}(\mathbf{y},\bar{\rho},\mathbf{0}) + \mathbf{J}\_{\rho}(\bar{\rho})\delta\rho + \frac{1}{2}\bar{\mathbf{H}}\_{\rho\rho}(\bar{\rho},\delta\rho)\delta\rho + \mathbf{v},\tag{2}$$

where  $\mathbf{J}\_{\rho}(\bar{\rho}) = \frac{\partial \mathbf{f}}{\partial \rho}|\_{\mathbf{y},\bar{\rho},\mathbf{0}}$  is the Jacobian of  $\mathbf{f}(\mathbf{y}, \rho, \mathbf{v})$  evaluated at  $\bar{\rho}$ , while   

 $\mathbf{\bar{H}}\_{\rho\rho}(\bar{\rho}, \delta\rho)$  is based on the Hessian of the individual entries of  $\mathbf{f}(\mathbf{y}, \rho, \mathbf{v})$ . Specifically,
$$\mathbf{J}\_{\rho}(\boldsymbol{\rho}) = \begin{bmatrix} \frac{\ell\_{1}^{\mathsf{T}}(\boldsymbol{\rho})}{\|\ell\_{1}(\boldsymbol{\rho})\|} \begin{bmatrix} \mathbf{1}\_{3\times3} & -\mathbf{C}\_{po}^{\mathsf{T}}(\boldsymbol{\theta})\mathbf{b}\_{1}^{\mathsf{X}}\mathbf{S}(\boldsymbol{\theta}) \end{bmatrix} \\ \vdots \\ \frac{\ell\_{m}^{\mathsf{T}}(\boldsymbol{\rho})}{\|\ell\_{m}(\boldsymbol{\rho})\|} \begin{bmatrix} \mathbf{1}\_{3\times3} & -\mathbf{C}\_{po}^{\mathsf{T}}(\boldsymbol{\theta})\mathbf{b}\_{m}^{\mathsf{X}}\mathbf{S}(\boldsymbol{\theta}) \end{bmatrix} \end{bmatrix}, \quad \bar{\mathbf{H}}\_{\rho\rho}(\boldsymbol{\rho},\boldsymbol{\delta}\boldsymbol{\rho}) = \begin{bmatrix} \boldsymbol{\delta}\boldsymbol{\rho}^{\mathsf{T}}\mathbf{H}\_{\rho\rho,1}(\boldsymbol{\rho}) \\ \vdots \\ \boldsymbol{\delta}\boldsymbol{\rho}^{\mathsf{T}}\mathbf{H}\_{\rho\rho,m}(\boldsymbol{\rho}) \end{bmatrix},$$

where  $J\_p(\rho)$  was derived in [\[11\]](#page-3-1) and the entries of  $\overline{H}\_{\rho\rho}(\rho, \delta\rho)$  are computed through the Hessian matrices
$$\mathbf{H}\_{\rho\rho,i}(\rho) = \begin{bmatrix} \frac{\partial^2 f\_i}{\partial \mathbf{r} \partial \mathbf{r}^\mathsf{T}} & \frac{\partial^2 f\_i}{\partial \theta \partial \mathbf{r}^\mathsf{T}}\\ \left(\frac{\partial^2 f\_i}{\partial \theta \partial \mathbf{r}^\mathsf{T}}\right)^\mathsf{T} & \frac{\partial^2 f\_i}{\partial \theta \partial \theta^\mathsf{T}} \end{bmatrix}, \quad i = 1, \cdots, m,$$

where

$$\frac{\partial^2 f\_i}{\partial \mathbf{r} \partial \mathbf{r}^\mathsf{T}} = \frac{\mathbf{1}\_{3 \times 3}}{\left\| \ell\_i(\rho) \right\|} - \frac{\ell\_i(\rho) \ell\_i^\mathsf{T}(\rho)}{\left\| \ell\_i(\rho) \right\|^3}, \quad \frac{\partial^2 f\_i}{\partial \theta \partial \mathbf{r}^\mathsf{T}} = \frac{\partial^2 f\_i}{\partial \mathbf{r} \partial \mathbf{r}^\mathsf{T}} (-\mathbf{C}\_{po}^\mathsf{T}(\theta) \mathbf{b}\_i^\times \mathbf{S}(\theta)),$$

$$\frac{\partial^2 f\_i}{\partial \theta \partial \theta^\mathsf{T}} = \mathbf{S}^\mathsf{T}(\theta) \mathbf{b}\_i^\times \mathbf{C}\_{po}(\theta) \frac{\partial^2 f\_i}{\partial \theta \partial \mathbf{r}^\mathsf{T}} + \frac{1}{\left\| \ell\_i(\bar{\rho}) \right\|} \frac{\partial}{\partial \theta} \left( \mathbf{S}^\mathsf{T}(\theta) \mathbf{b}\_i^\times \mathbf{C}\_{po}(\theta) \ell\_i(\bar{\rho}) \right).$$

The term  $S(\theta)$  is a mapping matrix based on the orientation kinematics  $\omega =$   
 $S(\theta)\dot{\theta}$ , where  $\omega$  is the angular velocity of  $\mathcal{F}\_p$  relative to  $\mathcal{F}\_o$  resolved in  $\mathcal{F}\_p$  and  
expressions for  $S(\theta)$  with different orientation parameterizations can be found  
in [\[11\]](#page-4-0). For the 3-2-1 Euler angle sequence used in this paper,  $S(\theta)$  is given by
$$\mathbf{S}(\boldsymbol{\theta}) = \begin{bmatrix} 1 & 0 & -\sin(\theta\_2) \\ 0 & \cos(\theta\_1) & \sin(\theta\_1)\cos(\theta\_2) \\ 0 & -\sin(\theta\_1) & \cos(\theta\_1)\cos(\theta\_2) \end{bmatrix}.$$

Note that the use of  $\ell\_i(\bar{\rho})$  in the final partial derivative of  $\frac{\partial^2 f\_i}{\partial \theta \partial \theta^\top}$  denotes that  $\ell\_i(\bar{\rho})$  is held constant in this partial derivative. The derivations of the terms within the Hessian matrix are provided in the Appendix. Note that the partial derivative  $\frac{\partial}{\partial \theta} \left(\mathbf{S}^\top(\theta) \mathbf{b}\_i^\times \mathbf{C}\_{po} \ell\_i(\bar{\rho})\right)$  in the term  $\frac{\partial^2 f\_i}{\partial \theta \partial \theta^\top}$  is not known to be representable through simple matrix expressions like the other partial derivatives. Instead, it can be derived for a given Euler angle sequence using a symbolic manipulation software package.#### 2.3 Forward Kinematics Algorithm 1: Halley's Method

A numerically-damped version of Halley's method inspired by the DQuIK method in [\[7\]](#page-4-0) begins with an initial end-effector pose estimate  $\rho^{(0)}$  and starts the *j<sup>th</sup>* iteration with the computation
$$\delta \boldsymbol{\rho}^{(j)} = \left(\mathbf{J}\_{\rho}^{\mathsf{T}}(\boldsymbol{\rho}^{(j)})\mathbf{J}\_{\rho}(\boldsymbol{\rho}^{(j)}) + \eta \mathbf{1}\right)^{-1} \mathbf{J}\_{\rho}^{\mathsf{T}}(\boldsymbol{\rho}^{(j)}) \mathbf{f}(\mathbf{y}, \boldsymbol{\rho}^{(j)}, \mathbf{0}\_{m \times 1}), \tag{3}$$

where  $\eta > 0$  is a tunable parameter that provides numerical damping to the calculation, especially if  $\rho^{(j)}$  is near a singularity. This is then followed by the update
$$\boldsymbol{\rho}^{(j+1)} = \boldsymbol{\rho}^{(j)} + \left(\bar{\mathbf{J}}\_{\rho}^{\mathsf{T}} \bar{\mathbf{J}}\_{\rho} + \eta \mathbf{1}\right)^{-1} \bar{\mathbf{J}}\_{\rho}^{\mathsf{T}} \mathbf{f}(\mathbf{y}, \boldsymbol{\rho}^{(j)}, \mathbf{0}\_{m \times 1}),\tag{4}$$

where  $\overline{\mathbf{J}}\_{\rho} = \mathbf{J}\_{\rho}(\rho^{(j)}) + \frac{1}{2}\overline{\mathbf{H}}(\rho^{(j)}, \delta\rho^{(j)})$ . The iteration index *j* is increased and the iteration proceeds until a specified stopping criterion is satisfied, such as  $\|\rho^{(j+1)} - \rho^{(j)}\|\_{2} < \epsilon$  or a maximum number of iterations is reached.The first step of Halley's method in (3) is derived by setting (2) equal to  $\mathbf{0}\_{m \times 1}$ , dropping the second-order term, and isolating for  $\delta \boldsymbol{\rho}$ . The second step in (4) is found through the same procedure with the inclusion of the second-order term in (2). It is worth noting that if only the second step in (4) is performed with  $\delta \boldsymbol{\rho}^{(j)} = \mathbf{0}\_{6 \times 1}$ , the damped version of Halley's method reverts to the Levenberg-Marquardt method.If it is desired to compute the covariance of the error in the pose estimate, for example to use the forward kinematics solution within a sensor fusion algorithm [\[5,10\]](#page-5-1), then (3) and (4) are respectively modified as
$$\delta\boldsymbol{\rho}^{(j)} = \left(\mathbf{J}\_{\rho}^{\mathrm{T}}(\boldsymbol{\rho}^{(j)})\mathbf{V}^{-1}\mathbf{J}\_{\rho}(\boldsymbol{\rho}^{(j)}) + \eta\mathbf{1}\right)^{-1}\mathbf{J}\_{\rho}^{\mathrm{T}}(\boldsymbol{\rho}^{(j)})\mathbf{V}^{-1}\mathbf{f}(\mathbf{y},\boldsymbol{\rho}^{(j)},\mathbf{0}\_{m\times 1}),$$

$$\boldsymbol{\rho}^{(j+1)} = \boldsymbol{\rho}^{(j)} + \left(\bar{\mathbf{J}}\_{\rho}^{\mathrm{T}}\mathbf{V}^{-1}\bar{\mathbf{J}}\_{\rho} + \eta\mathbf{1}\right)^{-1}\bar{\mathbf{J}}\_{\rho}^{\mathrm{T}}\mathbf{V}^{-1}\mathbf{f}(\mathbf{y},\boldsymbol{\rho}^{(j)},\mathbf{0}\_{m\times 1}),$$

where  $\mathbf{V} = \mathbb{E}[\mathbf{v}\mathbf{v}^{\mathsf{T}}] = \text{diag}\{\sigma\_1^2, ..., \sigma\_m^2\}$  is the measurement covariance. The covariance matrix is then computed at the  $j$ <sup>th</sup> iteration as [\[4, p. 84\]](#page-4-1)
$$\mathbf{P}^{(j)} = \mathbb{E}[(\rho\_{\text{true}} - \rho^{(j)})(\rho\_{\text{true}} - \rho^{(j)})^\mathsf{T}] = \left(\bar{\mathbf{J}}\_{\rho}^{\mathsf{T}} \mathbf{V}^{-1} \bar{\mathbf{J}}\_{\rho}\right)^{-1}.\tag{5}$$

If the measurement noise on each individual cable is the same (i.e.,  $\sigma\_i = \sigma$ ,  $i = 1,...,m$ , and  $\mathbf{V} = \sigma \mathbf{1}\_{m \times m}$ ), then (3) and (4) can be used directly and the covariance of the pose estimation error is simply  $\mathbf{P}^{(j)} = \sigma (\overline{\mathbf{J}}\_{\rho}^T \overline{\mathbf{J}}\_{\rho})^{-1}$ .### 2.4 Forward Kinematics Algorithm 2: Hybrid Method

A potential disadvantage of Halley's method in comparison to Newton's method  
or the Levenberg-Marquardt method is that it requires the additional computa-  
tion of the term  $\bar{H}\_{\rho\rho}(\bar{\rho}, \delta\rho)$  and it involves two matrix inversions within the same  
iteration by solving both (3) and (4). To reduce the need for unnecessary higher-  
order computations, a hybrid method is proposed that follows Halley's method  
for a prescribed number of iterations, then sets  $\bar{H}\_{\rho\rho}(\bar{\rho}, \delta\rho) = 0\_{m\times6}$  and follows  
the Levenberg-Marquardt for all subsequent iterations by only solving for (4).  
This proposed hybrid method is designed to take advantage of Halley's method's  
ability to converge relatively quickly in the presence of large initial pose estima-  
tion errors, as well as the computational efficiency of the Levenberg-Marquardt  
method when the pose estimation error becomes smaller in later iterations.

### 3 Results

The proposed forward kinematics algorithms are implemented in a numerical simulation of a 6-DOF suspended CDPR with 8 cables modeled after CoGiRo [\[19\]](#page-5-1) whose dimensions are provided in Table 1. All computations presented in this section are performed on a laptop computer with an AMD Ryzen 8945HS @ 4.0 GHz with 16 GB memory and the code is run in Python.A Monte-Carlo assessment is performed with end-effector poses randomly selected from uniform distributions  $\mathbf{r}\_{\text{min}} \leq \mathbf{r} \leq \mathbf{r}\_{\text{max}}$  and  $-\boldsymbol{\theta}\_{\text{max}} \leq \boldsymbol{\theta} \leq \boldsymbol{\theta}\_{\text{max}}$ , where  $\mathbf{r}\_{\text{min}} = \begin{bmatrix} -6.75 & -4.75 & 0 \end{bmatrix}^{\text{T}}$  m,  $\mathbf{r}\_{\text{max}} = \begin{bmatrix} 6.75 & 4.75 & 4.75 \end{bmatrix}^{\text{T}}$  m, and  $\boldsymbol{\theta}\_{\text{max}} = \begin{bmatrix} 30 & 30 & 30 \end{bmatrix}^{\text{T}}$  degrees are chosen based on a rough estimate of the bounds on the static workspace values of the CDPR. Each of these randomly-generated poses is assessed for static feasibility with a payload mass of 100 kg, a maximum cable

| Cable | Pulley position, $aiT$ (m) | Cable End-Effector Attachment Point, $biT$ (m) |
|-------|----------------------------|------------------------------------------------|
| 1     | [-7.20 -5.44 5.39]         | [0.503 -0.493 0.000]                           |
| 2     | [-7.48 -5.15 5.40]         | [-0.510 0.351 0.998]                           |
| 3     | [-7.41 5.19 5.40]          | [-0.503 -0.270 0.000]                          |
| 4     | [-7.12 5.47 5.41]          | [0.496 0.356 1.000]                            |
| 5     | [7.22 5.37 5.41]           | [-0.503 0.493 0.000]                           |
| 6     | [7.51 5.08 5.42]           | [0.500 -0.340 0.999]                           |
| 7     | [7.43 -5.26 5.39]          | [0.502 0.275 -0.001]                           |
| 8     | [7.14 -5.54 5.40]          | [-0.505 -0.346 0.998]                          |

Table 1. Dimensions used for the numerical simulations based on CoGiRo [19].

tension of 6,000 N, and a minimum cable tension of 10 N. This process is repeated until 10,000 statically-feasible poses are randomly generated. The inverse kinematics of each pose is then solved assuming straight and rigid cables to obtain the cable lengths associated with each pose. Perfect measurements with no noise are then used with the proposed forward kinematics algorithm and the Levenberg-Marquardt method using a stopping tolerance of  $\epsilon = 10^{-9}$ , a maximum of 30 iterations, numerical damping of  $\eta = 10^{-6}$ , and 3 iterations of Halley's method when using the proposed hybrid method. All methods are initialized with a pose estimate of  $\rho^{(0)} = \rho\_{true} + \delta\rho^{(0)}$ , where  $\rho\_{true}$  is the randomly-generated truth pose within the static feasible workspace and  $\delta\rho^{(0)} = {\begin{bmatrix} \delta\mathbf{r}^{(0)T} & \delta\mathbf{\theta}^{(0)T} \end{bmatrix}}^T$  is an initial perturbation. To assess the performance of these methods as a function of the initial pose estimation error,  $\delta\mathbf{r}^{(0)}$  and  $\delta\mathbf{\theta}^{(0)}$  are drawn from uniform distributions  $-\delta\mathbf{r}\_{max}^{(0)} \leq \delta\mathbf{r}^{(0)} \leq \delta\mathbf{r}\_{max}^{(0)}$  and  $-\delta\mathbf{\theta}\_{max}^{(0)} \leq \delta\mathbf{\theta}^{(0)} \leq \delta\mathbf{\theta}\_{max}^{(0)}$ . The Monte Carlo assessment is performed for each of the three forward kinematics algorithms with  $\mathbf{r}\_{max}^{(0)} = {\begin{bmatrix} 1 & 1 & 1 \end{bmatrix}}^T$  m and ten different values of  $\mathbf{\theta}\_{max}^{(0)}$  ranging from a small initial maximum orientation error of  $\mathbf{\theta}\_{max}^{(0)} = {\begin{bmatrix} 2 & 2 & 2 \end{bmatrix}}^T$  deg to a large initial maximum orientation error of  $\mathbf{\theta}\_{max}^{(0)} = {\begin{bmatrix} 40 & 40 & 40 \end{bmatrix}}^T$  deg.The results of this Monte-Carlo assessment are shown in Fig. 2, which include the mean and . 99th percentile of the number of iterations and computation time required to meet the specified stopping conditions, as well as the percentage of runs that successfully converge within .0.1 m and . 1 deg of the true end-effector pose. It is observed in Fig. 2 that for small initial pose estimation errors the proposed forward kinematics algorithms provide a small reduction in the number of iterations required and an increase in the computation time compared to the Levenberg-Marquardt method, with a small improvement in the success rate of accurate convergence. However, for large errors (roughly larger than .20 deg) the number of iterations is substantially decreased and the success rate is greatly increased with the proposed methods in comparison to the Levenberg-Marquardt method. Additionally, the . 99th-percentile computation times with the proposed hybrid method are reduced with large initial estimation errors compared to the

![](_page_7_Figure_1.jpeg)

Fig. 2. Monte-Carlo assessment of the proposed forward kinematics methods (labeled
"Halley" and "Hybrid"), as well as the Levenberg-Marquardt method (labeled "LM")
with no noise or cable elasticity, including (a) number of iterations, (b) computation
time required, and (c) success rate of convergence.

Levenberg-Marquardt method. Further assessment of the Monte Carlo simula-  

sions with the three methods is provided in Fig. 3, which includes the mean and  

99th-percentile errors in the final position and orientation estimates. These errors  

are computed as  $\|\mathbf{r}\_{true} - \mathbf{r}^{(j)}\|\_{2}$  and  $\|\boldsymbol{\theta}\_{true} - \boldsymbol{\theta}^{(j)}\|\_{2}$ , respectively, where  $j$  repre-  

sents the number of iterations required to meet the stopping criteria. Figure 3  

demonstrates that the estimation errors obtained with the proposed methods  

are considerably lower than those with the Levenberg-Marquardt method. Inter-  

estingly, even in situations where Fig. 2c shows less than a percent difference  

in the success rates between the three methods, there is a notable reduction in![](_page_8_Figure_1.jpeg)

Fig. 3. Monte-Carlo assessment of the proposed forward kinematics methods (labeled "Halley" and "Hybrid"), as well as the Levenberg-Marquardt method (labeled "LM") with no noise or cable elasticity, including (a) position estimation error and (b) orientation estimation error. Note that (b) uses a linear y-axis scale below . 5 deg and a logarithmic scale above this.

the orientation estimation error with the proposed methods compared to the Levenberg-Marquardt method. This seems to suggest that when the Levenberg-Marquardt method fails to converge, it tends to incur very large errors. In contrast, the proposed methods appear less prone to large errors, even when they fail to converge correctly.

To assess the robustness of the proposed algorithms to model uncertainty, measurement noise and cable elasticity are added to the simulation. In particular, the cable-length measurements are adjusted to be  $y\_i = ||l\_i(\rho)||\_2/(1+\frac{T\_i}{EA\_0}) + v\_i$ , where  $T\_i$  is the tension in the *i*<sup>th</sup> cable,  $E$  is the cable's elastic modulus, and  $A\_0$  is the cross-sectional area of the cable [\[9\]](#page-8-1). Numerical values of  $E = 35$  GPa and  $A\_0 = 8.205 \times 10^{-6}$  m<sup>2</sup> are used to match the CoGiRo values from [\[9\]](#page-8-1), while the zero-mean Gaussian white noise  $v\_i$  has a standard deviation of 5 mm. A quadratic program is solved to determine the cable tensions associated with each statically-feasible pose. The Monte-Carlo assessment is performed again with the noisy and elastic cable measurements and the results are included in Fig. 4. It is demonstrated in Fig. 4 that the trends seen with the idealized cable model in Fig. 2 also hold with elastic and noisy cable measurements. The only notable differences are that the number of iterations and computation time of all algorithms increases, the number of successful convergences decreases, and there is less of a distinction in successful convergence rate between the methods for small initial pose estimation error. This assessment provides confidence that the proposed algorithms can substantially improve the rate of successful convergence![](_page_9_Figure_1.jpeg)

Fig. 4. Monte-Carlo assessment of the proposed forward kinematics methods (labeled "Halley" and "Hybrid"), as well as the Levenberg-Marquardt method (labeled "LM") with noise and cable elasticity, including (a) number of iterations, (b) computation time required, and (c) success rate of convergence.

and in the case of the hybrid method reduce the computation time required in the presence of modeling errors and measurement uncertainty.

### 4 Conclusion

This paper presented two iterative forward kinematic algorithms for overconstrained CDPRs based on Halley's method that make use of second-order terms in the Taylor series expansion of the CDPR's loop-closure equations. It was shown through a numerical example of a suspended .6-DOF CDPR with. 8 cables that both proposed algorithms reduce the number of iterations required to reach a convergence tolerance and have a greater success rate of convergence to the true CDPR pose compared to the standard iterative Levenberg-Marquardt method. This trend is amplified as the initial pose estimation error increases. The proposed hybrid method also demonstrated quicker computation times compared to the Levenberg-Marquardt method in the presence of relatively large initial pose estimation errors. Future work on this topic will involve experimental implementation and validation of the proposed algorithms in a real-time framework, as well as comparisons to other forward kinematics algorithms in the literature.

Disclosure of Interests. The authors have no competing interests to declare that are relevant to the content of this article.

# Appendix: Derivation of Terms in Hessian

.

This appendix presents the derivation of the terms in the Hessian matrix from
in Sect. 2.2. Knowing that  $\frac{\partial^2 f\_i}{\partial \mathbf{r} \partial \mathbf{r}^\top} = \frac{\partial}{\partial \mathbf{r}} \left( \frac{\partial f\_i}{\partial \mathbf{r}} \right)^\top$ , the first Hessian term is
$$\begin{split} \frac{\partial^2 f\_i}{\partial \mathbf{r} \partial \mathbf{r}^\mathsf{T}} &= \frac{\partial}{\partial \mathbf{r}} \left( \frac{\ell\_i(\rho)}{\|\ell\_i(\rho)\|} \right) = \frac{\partial}{\partial \mathbf{r}} \left( \ell\_i(\rho) \right) \frac{1}{\|\ell\_i(\rho)\|} + \ell\_i(\rho) \frac{\partial}{\partial \mathbf{r}} \left( \frac{1}{\|\ell\_i(\rho)\|} \right) \\ &= \left( \mathbf{1}\_{3 \times 3} \right) \frac{1}{\|\ell\_i(\rho)\|} + \ell\_i(\rho) \left( -\frac{\ell\_i^\mathsf{T}(\rho)}{\|\ell\_i(\rho)\|^3} \right) = \frac{\mathbf{1}\_{3 \times 3}}{\|\ell\_i(\rho)\|} - \frac{\ell\_i(\rho) \ell\_i^\mathsf{T}(\rho)}{\|\ell\_i(\rho)\|^3}. \end{split}$$

Starting with  $\frac{\partial^2 f\_i}{\partial \theta \partial \mathbf{r}^\mathsf{T}} = \frac{\partial}{\partial \theta} \left( \frac{\partial f\_i}{\partial \mathbf{r}} \right)^\mathsf{T}$  and using the DCM identity  $\frac{\partial}{\partial \theta} (\mathbf{C}\_{po}^\mathsf{T}(\theta) \mathbf{u}) = -\mathbf{C}\_{po}^\mathsf{T}(\theta) \mathbf{u}^\times \mathbf{S}(\theta)$ ) [\[16\]](#page-10-573), the second Hessian term is computed as
$$\begin{split} \frac{\partial^2 f\_i}{\partial \boldsymbol{\theta} \partial \mathbf{r}^{\mathsf{T}}} &= \frac{\partial}{\partial \boldsymbol{\theta}} \left( \ell\_i(\boldsymbol{\rho}) \right) \frac{1}{\|\ell\_i(\boldsymbol{\rho})\|} + \ell\_i(\boldsymbol{\rho}) \frac{\partial}{\partial \boldsymbol{\theta}} \left( \frac{1}{\|\ell\_i(\boldsymbol{\rho})\|} \right) \\ &= \frac{\partial}{\partial \boldsymbol{\theta}} \left( \mathbf{C}\_{po}^{\mathsf{T}}(\boldsymbol{\theta}) \mathbf{b}\_i \right) \frac{1}{\|\ell\_i(\boldsymbol{\rho})\|} + \ell\_i(\boldsymbol{\rho}) \left( -\frac{\ell\_i^{\mathsf{T}}(\boldsymbol{\rho})}{\|\ell\_i(\boldsymbol{\rho})\|^3} \frac{\partial}{\partial \boldsymbol{\theta}} \left( \mathbf{C}\_{po}^{\mathsf{T}}(\boldsymbol{\theta}) \mathbf{b}\_i \right) \right) \\ &= -\left( \frac{\mathbf{1}\_{3 \times 3}}{\|\ell\_i(\boldsymbol{\rho})\|} - \frac{\ell\_i(\boldsymbol{\rho}) \ell\_i^{\mathsf{T}}(\boldsymbol{\rho})}{\|\ell\_i(\boldsymbol{\rho})\|^3} \right) \mathbf{C}\_{po}^{\mathsf{T}}(\boldsymbol{\theta}) \mathbf{b}\_i^{\mathsf{X}} \mathbf{S}(\boldsymbol{\theta}) = \frac{\partial^2 f\_i}{\partial \mathbf{r} \partial \mathbf{r}^{\mathsf{T}}} (-\mathbf{C}\_{po}^{\mathsf{T}}(\boldsymbol{\theta}) \mathbf{b}\_i^{\mathsf{X}} \mathbf{S}(\boldsymbol{\theta})). \end{split}$$

The final Hessian term is found starting with  $\frac{\partial^2 f\_i}{\partial \theta \partial \theta^\mathsf{T}} = \frac{\partial}{\partial \theta} \left( \frac{\partial f\_i}{\partial \theta} \right)^\mathsf{T}$ , resulting in
$$
\begin{split}
\frac{\partial^2 f\_i}{\partial \theta \partial \theta^{\mathsf{T}}} &= \mathbf{S}^{\mathsf{T}}(\theta) \mathbf{b}\_i^{\times} \mathbf{C}\_{po}(\theta) \ell\_i(\rho) \frac{\partial}{\partial \theta} \left( \frac{1}{\|\ell\_i(\rho)\|} \right) \\
&+ \frac{\partial}{\partial \theta} \left( \mathbf{S}^{\mathsf{T}}(\theta) \mathbf{b}\_i^{\times} \mathbf{C}\_{po}(\theta) \ell\_i(\rho) \right) \frac{1}{\|\ell\_i(\rho)\|} \\
&= -\mathbf{S}^{\mathsf{T}}(\theta) \mathbf{b}\_i^{\times} \mathbf{C}\_{po}(\theta) \left( \frac{\mathbf{1}\_{3 \times 3}}{\|\ell\_i(\rho)\|} - \frac{\ell\_i(\rho) \ell\_i^{\mathsf{T}}(\rho)}{\|\ell\_i(\rho)\|^3} \right) \mathbf{C}\_{po}^{\mathsf{T}}(\theta) \mathbf{b}\_i^{\times} \mathbf{S}(\theta) \\
&+ \frac{1}{\|\ell\_i(\bar{\rho})\|} \frac{\partial}{\partial \theta} \left( \mathbf{S}^{\mathsf{T}}(\theta) \mathbf{b}\_i^{\times} \mathbf{C}\_{po}(\theta) \ell\_i(\bar{\rho}) \right) \\
&= \mathbf{S}^{\mathsf{T}}(\theta) \mathbf{b}\_i^{\times} \mathbf{C}\_{po}(\theta) \frac{\partial^2 f\_i}{\partial \theta \partial \mathbf{r}^{\mathsf{T}}} + \frac{1}{\|\ell\_i(\bar{\rho})\|} \frac{\partial}{\partial \theta} \left( \mathbf{S}^{\mathsf{T}}(\theta) \mathbf{b}\_i^{\times} \mathbf{C}\_{po}(\theta) \ell\_i(\bar{\rho}) \right).
\end{split}
$$

# References

.

- 1. Baskar, A., Plecnik, M., Hauenstein, J.D., Wampler, C.W.: A numerical continuation approach using monodromy to solve the forward kinematics of cable-driven parallel robots with sagging cables. Mech. Mach. Theory 195, 105609 (2024)
- 2. Bieber, J., Pallmer, S., Beitelschmidt, M.: Computationally efficient implementation of the Gauss-Newton method for solving the forward kinematics of redundant cable-driven parallel robots. PMM-J. Appl. Math. Mech. 23(3), e202300231 (2023)
- 3. Caverly, R.J., Cheah, S.K., Bunker, K.R., Patel, S., Sexton, N., Nguyen, V.L.: Online self-calibration of cable-driven parallel robots using covariance-based data quality assessment metrics. J. Mech. Robot. 17(1), 010904 (2025)
- 4. Crassidis, J.L., Junkins, J.L.: Optimal Estimation of Dynamic Systems. CRC Press, Boca Raton (2004)
- 5. Garant, X., Campeau-Lecours, A., Cardou, P., Gosselin, C.: Improving the forward kinematics of cable-driven parallel robots through cable angle sensors. In: Cable-Driven Parallel Robots, pp. 167–179. Springer, Cham (2018)
- 6. Hughes, P.C.: Spacecraft Attitude Dynamics. Dover, Mineola (2004)
- 7. Lloyd, S., Irani, R.A., Ahmadi, M.: Fast and robust inverse kinematics of serial robots using Halley's method. IEEE T. Robot. 38(5), 2768–2780 (2022)
- 8. Merlet, J.P.: Some properties of the Irvine cable model and their use for the kinematic analysis of cable-driven parallel robots. Mech. Mach. Theory 135, 271–280 (2019)
- 9. Nguyen, D.Q., Gouttefarde, M., Company, O., Pierrot, F.: On the simplifications of cable model in static analysis of large-dimension cable-driven parallel robots. In: IEEE International Conference on Intelligent Robots, pp. 928–934 (2013)
- 10. Nguyen, V.L., Caverly, R.J.: Cable-driven parallel robot pose estimation using extended Kalman filtering with inertial payload measurements. IEEE Rob. Autom. Lett. 6(2), 3615–3622 (2021)
- 11. Nguyen, V.L., Caverly, R.J.: CDPR forward kinematics with error covariance bounds for unconstrained end-effector attitude parameterizations. In: International Conference on Cable-Driven Parallel Robots, pp. 37–49. Springer, Cham (2021)
- 12. Patel, S., Nguyen, V.L., Caverly, R.J.: Forward kinematics of a cable-driven parallel robot with pose estimation error covariance bounds. Mech. Mach. Theory 183, 105231 (2023)
- 13. Pott, A., Schmidt, V.: On the forward kinematics of cable-driven parallel robots. In: IEEE International Conference on Intelligent Robots, pp. 3182–3187 (2015)
- 14. Pott, A.: An algorithm for real-time forward kinematics of cable-driven parallel robots. In: Advances in Robot Kinematics: Motion in Man and Machine, pp. 529– 538. Springer, Dordrecht (2010)
- 15. Rheinboldt, W.C.: Methods for Solving Systems of Nonlinear Equations, 2nd edn. SIAM, Philadelphia (1998)
- 16. de Ruiter, A.H., Forbes, J.R.: General identities for parameterizations of SO(3) with applications. J. Appl. Mech. 81(7) (2014)
- 17. Santos, J.C., Gouttefarde, M.: A real-time capable forward kinematics algorithm for cable-driven parallel robots considering pulley kinematics. In: Advances in Robot Kinematics 2020, pp. 199–208. Springer, Cham (2021)
- 18. Scavo, T., Thoo, J.: On the geometry of Halley's method. Am. Math. Mon. 102(5), 417–426 (1995)
- 19. Tempel, P., Herve, P.E., Tempier, O., Gouttefarde, M., Pott, A.: Estimating inertial parameters of suspended cable-driven parallel robots—use case on CoGiRo. In: IEEE International Conference on Robotics and Automation, pp. 6093–6098 (2017)