<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>	
  <!-- Dark Blue Theme by Blue Cat Audio -->
  <VARIABLE id="THEME_PATH" value="$_DIR_$"/>
  
  <!-- Includes -->
	<INCLUDE_ONCE file="$THEME_PATH$/includes.inc" />
  
  <!-- Definitions of Generic types-->
 	<DEFINE>
    <!-- The main theme decorations, including the toolbar (optional)-->
    <THEME_DECORATIONS base_type="TPL_DECORATIONS"/>

    <!-- A cell that sets the default text style -->
    <THEME_TEXT_STYLE_CELL base_type="DECORATIONS_STYLE_CELL"/>

    <!-- Title bar, which can include additional buttons (in a row) -->
    <!-- Parameter: title (default is using the plug-in title metadata)-->
		<THEME_TITLE_BAR base_type="TPL_TITLE_BAR"/>
		
		<!-- Group Boxes -->
    <THEME_BOX base_type="TPL_IMAGE_OPACITY_BOX" image="$THEME_PATH$/group_boxes/pad-bluelight.png"/>
    <THEME_BOX_H_SEPARATOR base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/pad-bluelight_h_separator.png" stretch="true" width="100%" height="1"/>
    <THEME_BOX_V_SEPARATOR base_type="IMAGE_GROUP_BOX" image="$THEME_PATH$/group_boxes/pad-bluelight_v_separator.png" stretch="true" width="1" height="100%"/>

    <THEME_GROUP base_type="TPL_IMAGE_OPACITY_BOX" image="$THEME_PATH$/group_boxes/group.png"/>
    <THEME_GRAPH_BOX base_type="TPL_IMAGE_OPACITY_BOX" image="$THEME_PATH$/group_boxes/graph_box.bmp" h_margin="13" v_margin="5"/>

    <!-- Knobs -->
    <THEME_LARGE_KNOB base_type="CUS_LARGE_BLUE_KNOB"/>
    <THEME_LARGE_KNOB_SYM base_type="CUS_LARGE_BLUE_KNOB_SYM"/>
    <THEME_KNOB base_type="CUS_BLUE_KNOB"/>
    <THEME_KNOB_SYM base_type="CUS_BLUE_KNOB_SYM"/>
    <THEME_SMALL_KNOB base_type="CUS_SMALL_BLUE_KNOB"/>
    <THEME_SMALL_KNOB_SYM base_type="CUS_SMALL_BLUE_KNOB_SYM"/>
    <THEME_MINI_KNOB base_type="CUS_MINI_BLUE_KNOB"/>
    <THEME_MINI_KNOB_SYM base_type="CUS_MINI_BLUE_KNOB_SYM"/>

    <THEME_KNOB_CHANNEL1 base_type="CUS_BLUE_KNOB"/>
    <THEME_KNOB_CHANNEL1_SYM base_type="CUS_BLUE_KNOB_SYM"/>
    <THEME_MINI_KNOB_CHANNEL1 base_type="CUS_MINI_BLUE_KNOB"/>
    
    <THEME_KNOB_CHANNEL2 base_type="CUS_RED_KNOB"/>
    <THEME_KNOB_CHANNEL2_SYM base_type="CUS_RED_KNOB_SYM"/>
    <THEME_MINI_KNOB_CHANNEL2 base_type="CUS_MINI_RED_KNOB"/>
    
    <!-- Sliders -->
    <THEME_MINI_H_SLIDER base_type="IMAGE_PARAM_SLIDER" orientation="horizontal" image="$THEME_PATH$/sliders/h_slider_small.bmp" cursor="system::size_h"/>
    <THEME_MINI_V_SLIDER base_type="IMAGE_PARAM_SLIDER" orientation="vertical" image="$THEME_PATH$/sliders/v_slider_small.bmp" cursor="system::size_v"/>

    <!-- Meters -->
    <THEME_ROUND_METER base_type="TPL_ROUND_METER"/>

    <!-- General Purpose Param Buttons -->
    <THEME_ON_OFF_BUTTON base_type="CUS_ON_OFF_BUTTON"/>
    <THEME_CHECK_BOX base_type="CUS_CHECK_BOX"/>
    <THEME_STATUS_BUTTON_80 base_type="CUS_STATUS_BUTTON_80"/>
    
    <!-- Switches -->
    <THEME_V_SELECT_SWITCH base_type="CUS_V_SELECT_SWITCH"/>
    <THEME_H_SELECT_SWITCH base_type="CUS_H_SELECT_SWITCH"/>

    <!-- Icon Buttons -->
    <THEME_SHOW_CONTROLS_BUTTON base_type="CUS_SHOW_CONTROLS_BUTTON"/>
    <THEME_SHOW_METERS_BUTTON   base_type="CUS_SHOW_METERS_BUTTON"/>
    <THEME_SHOW_GRAPH_BUTTON    base_type="CUS_SHOW_GRAPH_BUTTON"/>
    <THEME_MAGNIFY_BUTTON       base_type="CUS_MAGNIFY_BUTTON"/>
    <THEME_REFRESH_PARAM_BUTTON base_type="CUS_REFRESH_PARAM_BUTTON"/>
    <THEME_EDIT_ACTION_BUTTON   base_type="CUS_EDIT_ACTION_BUTTON"/>
    <THEME_FOLDER_ACTION_BUTTON   base_type="CUS_FOLDER_ACTION_BUTTON"/>      
    <THEME_POWER_BUTTON         base_type="IMAGE_PARAM_BUTTON" image="$THEME_PATH$/titlebar/power_button.png" image_hover="$THEME_PATH$/titlebar/power_button_hi.png"  image_focus="$THEME_PATH$/titlebar/power_button_hi.png" cursor="system::hand"/>

      <!-- Dropdown buttons -->
    <THEME_DROPDOWN_PARAM_BUTTON_80 base_type="CUS_DROPDOWN_PARAM_BUTTON_80"/>
    <THEME_DROPDOWN_PARAM_BUTTON_90 base_type="CUS_DROPDOWN_PARAM_BUTTON_90"/>
    <THEME_DROPDOWN_ACTION_BUTTON_80 base_type="CUS_DROPDOWN_ACTION_BUTTON_80"/>
    <THEME_DROPDOWN_ACTION_BUTTON_90 base_type="CUS_DROPDOWN_ACTION_BUTTON_90"/>
    
    <!-- Settings buttons -->
    <!-- Parameter: param_id (to be used only on dsp parameters)-->
    <THEME_DSP_PARAM_ROUND_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_name="settings_round"/>
    <THEME_DSP_PARAM_SMALL_ROUND_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_name="settings_round_small"/>
    <THEME_DSP_PARAM_SMALL_RECT_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_name="settings_rect_small"/>
    <THEME_DSP_PARAM_MINI_SETTINGS_BUTTON base_type="TPL_DSP_PARAM_SETTINGS_BUTTON" image_name="settings_mini"/>

	</DEFINE>
</DUMMY>