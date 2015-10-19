<?xml version="1.0" encoding="utf-8" ?>
<TEMPLATE id="TPL_DSP_PARAM_SETTINGS_BUTTON" param_id="#UNDEFINED#" button_index="" image_name="#UNDEFINED#" h_offset="0" v_offset="0" h_align="center" v_align="center">
    <!-- Action button to change the control settings for a dsp parameter -->
    <!-- image name defines the name of the image in the settings_buttons folder -->
    <IMAGE_ACTION_BUTTON action_id="Show_$param_id$_ControlSetup" cursor="system::hand" image="$THEME_PATH$/settings_buttons/$image_name$.png" image_hover="$THEME_PATH$/settings_buttons/$image_name$.png" image_focus="$THEME_PATH$/settings_buttons/$image_name$.png" image_pushed="$THEME_PATH$/settings_buttons/$image_name$_pushed.png" id="$param_id$_settings_button$button_index$" h_align="$h_align$" v_align="$v_align$" h_offset="$h_offset$" v_offset="$v_offset$"  ignore_mask_files="true">
      <TEMPLATE_INNER_CONTENT/>
    </IMAGE_ACTION_BUTTON> 
    <PARAM_LINK from="theme.show_settings" to="$param_id$_settings_button$button_index$.visible"/>
</TEMPLATE>