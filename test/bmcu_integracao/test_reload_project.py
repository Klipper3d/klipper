import json
import unittest
import xml.etree.ElementTree as xml
import zipfile
from collections import Counter

from test_integracao import ROOT


class ReloadProjectTests(unittest.TestCase):
    def test_project_has_four_cycles_of_two_layers_per_channel(self):
        path = ROOT / (
            'profiles/orca/ender3-v3-se-bmcu/'
            'teste_4_canais_3_recargas_PETG.3mf'
        )
        with zipfile.ZipFile(path) as archive:
            settings = json.loads(
                archive.read('Metadata/project_settings.config')
            )
            model = xml.fromstring(archive.read('3D/3dmodel.model'))
            config = xml.fromstring(
                archive.read('Metadata/model_settings.config')
            )
        ns = {'m': 'http://schemas.microsoft.com/3dmanufacturing/core/2015/02'}
        parts = config.findall('object/part')
        self.assertEqual(len(parts), 16)
        channels = []
        for index, part in enumerate(parts):
            channels.append(
                int(part.find("metadata[@key='extruder']").get('value'))
            )
            obj = model.find(
                "m:resources/m:object[@id='%s']" % part.get('id'), ns
            )
            heights = [
                float(v.get('z'))
                for v in obj.findall('m:mesh/m:vertices/m:vertex', ns)
            ]
            self.assertAlmostEqual(min(heights), index * 0.4)
            self.assertAlmostEqual(max(heights), (index + 1) * 0.4)
        self.assertEqual(channels, [1, 2, 3, 4] * 4)
        self.assertEqual(Counter(channels), {1: 4, 2: 4, 3: 4, 4: 4})
        self.assertEqual(settings['layer_height'], '0.20')
        self.assertEqual(settings['enable_prime_tower'], '0')
        self.assertEqual(settings['nozzle_temperature'], ['230'] * 4)
        self.assertNotIn('UID=', settings['machine_start_gcode'])
        for channel in range(4):
            self.assertIn(
                'TOOL=%d DEVICE=bmcu0 CHANNEL=%d' % (channel + 1, channel),
                settings['machine_start_gcode'],
            )
        self.assertEqual(settings['post_process'], [])
        self.assertEqual(settings['print_host'], '')


if __name__ == '__main__':
    unittest.main()
