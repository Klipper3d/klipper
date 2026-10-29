import argparse
import json
import xml.etree.ElementTree as xml
import zipfile
from pathlib import Path

CORE = 'http://schemas.microsoft.com/3dmanufacturing/core/2015/02'
xml.register_namespace('', CORE)


def element(parent, tag, **attributes):
    return xml.SubElement(parent, '{%s}%s' % (CORE, tag), attributes)


def metadata(parent, key, value):
    xml.SubElement(parent, 'metadata', key=key, value=str(value))


def generate(base, output):
    with zipfile.ZipFile(base) as archive:
        settings = json.loads(archive.read('Metadata/project_settings.config'))
    settings.update(
        {
            'layer_height': '0.20',
            'initial_layer_print_height': '0.20',
            'enable_prime_tower': '0',
            'enable_filament_ramming': '0',
            'single_extruder_multi_material': '1',
            'single_extruder_multi_material_priming': '0',
            'print_sequence': 'by layer',
            'wall_loops': '2',
            'sparse_infill_density': '100%',
            'sparse_infill_pattern': 'rectilinear',
            'top_shell_layers': '2',
            'bottom_shell_layers': '2',
            'enable_support': '0',
            'brim_type': 'no_brim',
            'outer_wall_speed': '40',
            'inner_wall_speed': '50',
            'sparse_infill_speed': '50',
            'internal_solid_infill_speed': '50',
            'top_surface_speed': '35',
            'initial_layer_speed': '20',
            'initial_layer_infill_speed': '25',
            'default_acceleration': '1000',
            'outer_wall_acceleration': '800',
            'initial_layer_acceleration': '500',
            'nozzle_temperature': ['230'] * 4,
            'nozzle_temperature_initial_layer': ['230'] * 4,
            'filament_type': ['PETG'] * 4,
            'filament_settings_id': [
                'PETG BMCU - channel %d' % i for i in range(1, 5)
            ],
            'filament_colour': ['#400000', '#FF0000', '#804000', '#FFFFFF'],
            'enable_pressure_advance': ['1'] * 4,
            'pressure_advance': ['0.04'] * 4,
            'filament_flow_ratio': ['1'] * 4,
            'filament_max_volumetric_speed': ['12'] * 4,
            'filament_start_gcode': [''] * 4,
            'filament_end_gcode': [''] * 4,
            'machine_end_gcode': 'ENDER_FINALIZAR_MULTICOR',
            'change_filament_gcode': 'T{next_extruder} B{next_extruder+1}',
            'machine_pause_gcode': 'PAUSE',
            'post_process': [],
            'print_settings_id': 'BMCU - four channels, three reloads each',
            'print_host': '',
            'print_host_webui': '',
            'printhost_apikey': '',
            'printhost_user': '',
            'printhost_password': '',
            'notes': '',
        }
    )
    for plate in ('hot_plate', 'textured_plate', 'cool_plate', 'eng_plate'):
        settings[plate + '_temp_initial_layer'] = ['80'] * 4
        settings[plate + '_temp'] = ['75'] * 4
    start = ['BMCU_PRINT_BEGIN SCHEMA=1 RESET=1']
    for channel in range(4):
        start.append(
            'BMCU_PRINT_MAP TOOL=%d DEVICE=bmcu0 CHANNEL=%d '
            'MATERIAL="{filament_type[%d]}" COLOR="{filament_colour[%d]}"'
            % (channel + 1, channel, channel, channel)
        )
    start.extend(
        [
            'BMCU_PRINT_COMMIT',
            'ENDER_INICIAR_MULTICOR '
            'TEMP_MESA=[bed_temperature_initial_layer_single] '
            'TEMP_BICO=[nozzle_temperature_initial_layer] '
            'FERRAMENTA={initial_tool+1}',
            'T{initial_tool} B{initial_tool+1}',
        ]
    )
    settings['machine_start_gcode'] = '\n'.join(start)
    model = xml.Element('{%s}model' % CORE, unit='millimeter')
    element(model, 'metadata', name='Application').text = 'OrcaSlicer-2.4.2'
    element(model, 'metadata', name='Title').text = (
        'BMCU four-channel reload test'
    )
    resources = element(model, 'resources')
    configuration = xml.Element('config')
    assembly = xml.SubElement(configuration, 'object', id='17')
    metadata(assembly, 'name', 'Teste BMCU - 3 recargas por canal')
    metadata(assembly, 'extruder', 1)
    triangles = (
        (0, 2, 1),
        (0, 3, 2),
        (4, 5, 6),
        (4, 6, 7),
        (0, 1, 5),
        (0, 5, 4),
        (1, 2, 6),
        (1, 6, 5),
        (2, 3, 7),
        (2, 7, 6),
        (3, 0, 4),
        (3, 4, 7),
    )
    for index in range(16):
        identifier = str(index + 1)
        channel = index % 4 + 1
        low, high = index * 0.4, (index + 1) * 0.4
        obj = element(resources, 'object', id=identifier, type='model')
        mesh = element(obj, 'mesh')
        vertices = element(mesh, 'vertices')
        for z in (low, high):
            for x, y in ((-12, -8), (12, -8), (12, 8), (-12, 8)):
                element(vertices, 'vertex', x=str(x), y=str(y), z='%.6f' % z)
        facets = element(mesh, 'triangles')
        for v1, v2, v3 in triangles:
            element(facets, 'triangle', v1=str(v1), v2=str(v2), v3=str(v3))
        part = xml.SubElement(
            assembly, 'part', id=identifier, subtype='normal_part'
        )
        metadata(
            part, 'name', 'Ciclo %d - canal %d' % (index // 4 + 1, channel)
        )
        metadata(part, 'extruder', channel)
    combined = element(resources, 'object', id='17', type='model')
    components = element(combined, 'components')
    for identifier in range(1, 17):
        element(components, 'component', objectid=str(identifier))
    build = element(model, 'build')
    element(
        build, 'item', objectid='17', transform='1 0 0 0 1 0 0 0 1 110 110 0'
    )
    plate = xml.SubElement(configuration, 'plate')
    metadata(plate, 'plater_id', 1)
    metadata(plate, 'plater_name', 'BMCU - 16 cargas / 15 trocas')
    metadata(plate, 'locked', 'false')
    instance = xml.SubElement(plate, 'model_instance')
    metadata(instance, 'object_id', 17)
    metadata(instance, 'instance_id', 0)
    metadata(instance, 'identify_id', 1)
    content_types = (
        '<?xml version="1.0" encoding="UTF-8"?>'
        '<Types xmlns="http://schemas.openxmlformats.org/package/2006/'
        'content-types">'
        '<Default Extension="rels" ContentType="application/'
        'vnd.openxmlformats-package.relationships+xml"/>'
        '<Default Extension="model" ContentType="application/'
        'vnd.ms-package.3dmanufacturing-3dmodel+xml"/>'
        '<Default Extension="config" ContentType="application/xml"/>'
        '</Types>'
    )
    relationships = (
        '<?xml version="1.0" encoding="UTF-8"?>'
        '<Relationships xmlns="http://schemas.openxmlformats.org/package/'
        '2006/relationships">'
        '<Relationship Target="/3D/3dmodel.model" Id="rel0" '
        'Type="http://schemas.microsoft.com/3dmanufacturing/2013/01/3dmodel"/>'
        '</Relationships>'
    )
    output.parent.mkdir(parents=True, exist_ok=True)
    with zipfile.ZipFile(output, 'w', zipfile.ZIP_DEFLATED) as archive:
        archive.writestr('[Content_Types].xml', content_types)
        archive.writestr('_rels/.rels', relationships)
        archive.writestr(
            '3D/3dmodel.model',
            xml.tostring(model, encoding='utf-8', xml_declaration=True),
        )
        archive.writestr(
            'Metadata/model_settings.config',
            xml.tostring(
                configuration, encoding='utf-8', xml_declaration=True
            ),
        )
        archive.writestr(
            'Metadata/project_settings.config',
            json.dumps(settings, ensure_ascii=False, indent=2),
        )
    print(output)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--base-project', type=Path, required=True)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    generate(args.base_project, args.output)


if __name__ == '__main__':
    main()
