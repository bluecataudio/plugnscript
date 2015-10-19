<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
<TEMPLATE id="TPL_HSWITCH" param_id="#UNDEFINED#" reverse="false">
    <COLUMN h_margin="-12">
      <IMAGE_PARAM_BUTTON image="$THEME_PATH$/switches/switch_button.png" cursor="system::hand" param_id="$param_id$" reverse="$reverse$"/>
      <CELL height="-11" />
    </COLUMN>
</TEMPLATE>
<DEFINE>
  <CUS_V_SELECT_SWITCH base_type="IMAGE_PARAM_BUTTON" image="$THEME_PATH$/switches/v_select_switch.png" image_focus="$THEME_PATH$/switches/v_select_switch.png" image_hover="$THEME_PATH$/switches/v_select_switch.png" image_pushed="$THEME_PATH$/switches/v_select_switch_pushed.png" cursor="system::hand" ignore_mask_files="true"/>
  <CUS_H_SELECT_SWITCH base_type="IMAGE_PARAM_BUTTON" image="$THEME_PATH$/switches/h_select_switch.png" image_focus="$THEME_PATH$/switches/h_select_switch.png" image_hover="$THEME_PATH$/switches/h_select_switch.png" image_pushed="$THEME_PATH$/switches/h_select_switch_pushed.png" cursor="system::hand" ignore_mask_files="true"/>
</DEFINE>
</DUMMY>