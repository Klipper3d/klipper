# Log data analyzing functions
#
# Copyright (C) 2021  Kevin O'Connor <kevin@koconnor.net>
#
# This file may be distributed under the terms of the GNU GPLv3 license.
import collections


######################################################################
# Analysis code
######################################################################

# Analyzer handlers: {name: class, ...}
AHandlers = {}

# Calculate a derivative (position to velocity, or velocity to accel)
class GenDerivative:
    DataSets = [
        ('derivative:<dataset>', 'Derivative of the given dataset'),
    ]
    def __init__(self, amanager, params):
        self.amanager = amanager
        self.source = params
        amanager.setup_dataset(self.source)
    def get_label(self):
        label = self.amanager.get_label(self.source)
        lname = label['label']
        units = label['units']
        if '(mm)' in units:
            rep = [('Position', 'Velocity'), ('(mm)', '(mm/s)')]
        elif '(mm/s)' in units:
            rep = [('Velocity', 'Acceleration'), ('(mm/s)', '(mm/s^2)')]
        else:
            return {'label': 'Derivative', 'units': 'Unknown'}
        for old, new in rep:
            lname = lname.replace(old, new).replace(old.lower(), new.lower())
            units = units.replace(old, new).replace(old.lower(), new.lower())
        return {'label': lname, 'units': units}
    def generate_data(self):
        inv_seg_time = 1. / self.amanager.get_segment_time()
        data = self.amanager.get_datasets()[self.source]
        deriv = [(data[i+1] - data[i]) * inv_seg_time
                 for i in range(len(data)-1)]
        return [deriv[0]] + deriv
AHandlers["derivative"] = GenDerivative

# Calculate a kinematic stepper position from the toolhead requested position
class GenKinematicPosition:
    DataSets = [
        ('kin:<stepper>', 'Stepper position derived from toolhead kinematics'),
    ]
    def __init__(self, amanager, params):
        self.amanager = amanager
        status = self.amanager.get_initial_status()
        kin = status['configfile']['settings']['printer']['kinematics']
        if kin not in ['cartesian', 'corexy']:
            raise amanager.error("Unsupported kinematics '%s'" % (kin,))
        if params not in ['stepper_x', 'stepper_y', 'stepper_z']:
            raise amanager.error("Unknown stepper '%s'" % (params,))
        if kin == 'corexy' and params in ['stepper_x', 'stepper_y']:
            self.source1 = 'trapq:toolhead:x'
            self.source2 = 'trapq:toolhead:y'
            if params == 'stepper_x':
                self.generate_data = self.generate_data_corexy_plus
            else:
                self.generate_data = self.generate_data_corexy_minus
            amanager.setup_dataset(self.source1)
            amanager.setup_dataset(self.source2)
        else:
            self.source1 = 'trapq:toolhead:' + params[-1:]
            self.source2 = None
            self.generate_data = self.generate_data_passthrough
            amanager.setup_dataset(self.source1)
    def get_label(self):
        return {'label': 'Position', 'units': 'Position\n(mm)'}
    def generate_data_corexy_plus(self):
        datasets = self.amanager.get_datasets()
        data1 = datasets[self.source1]
        data2 = datasets[self.source2]
        return [d1 + d2 for d1, d2 in zip(data1, data2)]
    def generate_data_corexy_minus(self):
        datasets = self.amanager.get_datasets()
        data1 = datasets[self.source1]
        data2 = datasets[self.source2]
        return [d1 - d2 for d1, d2 in zip(data1, data2)]
    def generate_data_passthrough(self):
        return self.amanager.get_datasets()[self.source1]
AHandlers["kin"] = GenKinematicPosition

# Calculate a position deviation
class GenDeviation:
    DataSets = [
        ('deviation:<dataset1>-<dataset2>', 'Difference between datasets'),
    ]
    def __init__(self, amanager, params):
        self.amanager = amanager
        parts = params.split('-')
        if len(parts) != 2:
            raise amanager.error("Invalid deviation '%s'" % (params,))
        self.source1, self.source2 = parts
        amanager.setup_dataset(self.source1)
        amanager.setup_dataset(self.source2)
    def get_label(self):
        label1 = self.amanager.get_label(self.source1)
        label2 = self.amanager.get_label(self.source2)
        if label1['units'] != label2['units']:
            return {'label': 'Deviation', 'units': 'Unknown'}
        parts = label1['units'].split('\n')
        units = '\n'.join([parts[0]] + ['Deviation'] + parts[1:])
        return {'label': label1['label'] + ' deviation', 'units': units}
    def generate_data(self):
        datasets = self.amanager.get_datasets()
        data1 = datasets[self.source1]
        data2 = datasets[self.source2]
        return [d1 - d2 for d1, d2 in zip(data1, data2)]
AHandlers["deviation"] = GenDeviation


######################################################################
# List datasets
######################################################################

def list_datasets():
    datasets = []
    for ah in sorted(AHandlers.keys()):
        datasets += AHandlers[ah].DataSets
    return datasets


######################################################################
# Data generation
######################################################################

# Manage raw and generated data samples
class AnalyzerManager:
    error = None
    def __init__(self, lmanager, segment_time):
        self.lmanager = lmanager
        self.error = lmanager.error
        self.segment_time = segment_time
        self.raw_datasets = collections.OrderedDict()
        self.gen_datasets = collections.OrderedDict()
        self.datasets = {}
        self.dataset_times = []
        self.duration = 5.
    def set_duration(self, duration):
        self.duration = duration
    def get_segment_time(self):
        return self.segment_time
    def get_datasets(self):
        return self.datasets
    def get_dataset_times(self):
        return self.dataset_times
    def get_initial_status(self):
        return self.lmanager.get_initial_status()
    def setup_dataset(self, name):
        name = name.strip()
        if name in self.raw_datasets:
            return self.raw_datasets[name]
        if name in self.gen_datasets:
            return self.gen_datasets[name]
        nparts = name.split(':')
        if nparts[0] in self.lmanager.available_dataset_types():
            hdl = self.lmanager.setup_dataset(name)
            self.raw_datasets[name] = hdl
        else:
            cls = AHandlers.get(nparts[0])
            if cls is None:
                raise self.error("Unknown dataset '%s'" % (name,))
            hdl = cls(self, ':'.join(nparts[1:]))
            self.gen_datasets[name] = hdl
        self.datasets[name] = []
        return hdl
    def get_label(self, dataset):
        hdl = self.raw_datasets.get(dataset)
        if hdl is None:
            hdl = self.gen_datasets.get(dataset)
            if hdl is None:
                raise error("Unknown dataset '%s'" % (dataset,))
        return hdl.get_label()
    def generate_datasets(self):
        # Generate raw data
        list_hdls = [(self.datasets[name], hdl)
                     for name, hdl in self.raw_datasets.items()]
        initial_start_time = self.lmanager.get_initial_start_time()
        start_time = t = self.lmanager.get_start_time()
        end_time = start_time + self.duration
        while t < end_time:
            t += self.segment_time
            self.dataset_times.append(t - initial_start_time)
            for dl, hdl in list_hdls:
                dl.append(hdl.pull_data(t))
        # Generate analyzer data
        for name, hdl in self.gen_datasets.items():
            self.datasets[name] = hdl.generate_data()
