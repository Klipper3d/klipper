BMCU_PRINT_BEGIN SCHEMA=1 RESET=1
{if is_extruder_used[0]}
BMCU_PRINT_MAP TOOL=1 UID=YOUR_BMCU_UID CHANNEL=0 MATERIAL="{filament_type[0]}" COLOR="{filament_colour[0]}"
{endif}
{if is_extruder_used[1]}
BMCU_PRINT_MAP TOOL=2 UID=YOUR_BMCU_UID CHANNEL=1 MATERIAL="{filament_type[1]}" COLOR="{filament_colour[1]}"
{endif}
{if is_extruder_used[2]}
BMCU_PRINT_MAP TOOL=3 UID=YOUR_BMCU_UID CHANNEL=2 MATERIAL="{filament_type[2]}" COLOR="{filament_colour[2]}"
{endif}
{if is_extruder_used[3]}
BMCU_PRINT_MAP TOOL=4 UID=YOUR_BMCU_UID CHANNEL=3 MATERIAL="{filament_type[3]}" COLOR="{filament_colour[3]}"
{endif}
BMCU_PRINT_COMMIT
ENDER_INICIAR_MULTICOR TEMP_MESA=[bed_temperature_initial_layer_single] TEMP_BICO=[nozzle_temperature_initial_layer] FERRAMENTA={initial_tool+1}
T{initial_tool} B{initial_tool+1}
