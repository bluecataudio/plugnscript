<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
  <INCLUDE_ONCE file="language_extensions/language_extensions.inc"/>
  <INCLUDE_ONCE file="mapping.inc"/>
  <INCLUDE_ONCE file="$_SYSTEM_TYPE_$.inc"/>
  <DEFINE>
    <CUS_LAYOUT_BUTTON base_type="IMAGE_PARAM_BUTTON" image="screen_layout_button.png" image_focus="screen_layout_button_hover.png" cursor="system::hand" images_count="2"/>
  </DEFINE>
  <VARIABLE id="minimum_width" value="280"/>
  <VARIABLE id="minimum_height" value="150"/>
  <VARIABLE id="spacing" value="4"/>
  
  <VARIABLE id="RULER_SUBDIV_COLOR" value="#333333"/>
  <VARIABLE id="RULER_COLOR" value="#777777"/>

    <STRING id="PlugNScript.script_name" default="No Script"/>
  <ACTION id="PlugNScript.ShowScriptsMenu" type="DisplayPlugNScriptMenu" script_file_name_string_id="PlugNScript.script_name"/>
  <ACTION id="OpenScriptFile" type="OpenUrl" url_string_id="$script_file_path$" application_string_id="global.edit_script_application"/>
  <ACTION id="OpenLogFile" type="OpenUrl" url_string_id="$log_file_path$"  application_string_id="global.open_log_application"/>
  <ACTION id="OpenScriptDirectory" type="OpenUrl" url_string_id="$script_file_path$" application="$FILE_EXPLORER_APP$"/>

  <REPEAT count="$SCRIPT_PARAMS_COUNT$">
    <!-- Input param for display and control, linked to actual DSP param-->
    <PARAM id="custom_param$index$" min="0" max="1" default="0" name="Param $index$"/>
    <PARAM_LINK from="$script_input_param$index$_min$" to="custom_param$index$.min"/>
    <PARAM_LINK from="$script_input_param$index$_max$" to="custom_param$index$.max"/>
    <PARAM_LINK from="$script_input_param$index$_default$" to="custom_param$index$.default"/>
    <STRING_LINK from="$script_input_param$index$_unit$" to="custom_param$index$.unit"/>
    <STRING_LINK from="$script_input_param$index$_name$" to="custom_param$index$.name"/>
    <STRING_LINK from="$script_input_param$index$_enum_values$" to="custom_param$index$.enum_values"/>
    <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control$index$.positions_count"/>
    <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control_menu$index$.positions_count"/>
    <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control$index$.visible" formula="x!=2"/>
    <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control_switch$index$.visible" formula="x==2"/>
    <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control_menu$index$.visible" formula="(x>=2)and(x&lt;=50)"/>

    <PARAM_LINK from="$script_input$index$$" to="custom_param$index$" normalized="true" enabled="true"/>
    <PARAM_LINK from="custom_param$index$" to="$script_input$index$$" enabled="false" id="link$index$" normalized="true"/>
    <PARAM_LINK from="custom_param$index$.capturing" to="link$index$.enabled"/>
   </REPEAT>
   <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$">
    <!-- Output param for display, linked to actual DSP param-->
    <PARAM id="custom_out_param$index$" min="0" max="1" default="0" name="Out Param $index$"/>
    <PARAM_LINK from="$script_output_param$index$_min$" to="custom_out_param$index$.min"/>
    <PARAM_LINK from="$script_output_param$index$_max$" to="custom_out_param$index$.max"/>
    <PARAM_LINK from="$script_output_param$index$_default$" to="custom_out_param$index$.default"/>
    <STRING_LINK from="$script_output_param$index$_unit$" to="custom_out_param$index$.unit"/>
    <STRING_LINK from="$script_output_param$index$_name$" to="custom_out_param$index$.name"/>
    <STRING_LINK from="$script_output_param$index$_enum_values$" to="custom_out_param$index$.enum_values"/>
    <PARAM_LINK from="$script_output$index$$" to="custom_out_param$index$" normalized="true"/>
  </REPEAT>
  <PARAM id="thin_display" default="1" type="boolean" persistent="true"/>

  <!-- Custom widgets definitions -->
  <!-- Level meter -->
    <TEMPLATE id="TPL_CHANNEL_METER" param_id="#UNDEFINED#" hold_param_id="#UNDEFINED#" ruler_after="0" direction="input">
        <!-- determines the delimiter between main bus meter and others: negative if nothing to show-->
        <FORMULA_PARAM id="main_bus_$direction$_delimiter_index" formula="main_bus_channels_count*(1-2*(main_bus_channels_count&gt;=dsp.connected_$direction$s_count))"/>
        <ROW spacing="1" flex="1">
            <PARAM_LINK from="$param_id$_meter.ruler.height" to="$param_id$_meter.ruler.positions_count" formula="(x&lt;120)*6+((x&gt;=120)&amp;&amp;(x&lt;400))*11+(x&gt;=400)*21"/>
            <PARAM_LINK from="$param_id$_meter.ruler.height" to="$param_id$_meter.ruler.visible" formula="(x&gt;=25)"/>
            <REPEAT count="$ruler_after$==0">
                <RULER id="$param_id$_meter.ruler" param_info_id="$$param_id$1$.info" positions_count="11" height="100%" text_v_align="center" v_margin="6" width="15" orientation="vertical" content="{value}" value_format=".0" text_color="$RULER_COLOR$" font_size="11"/>
            </REPEAT>
            <ROW  v_margin="6" height="100%">
                <GRID y_subdiv_count="1" height="100%" subdiv_color="$RULER_SUBDIV_COLOR$" origin_color="#666666" color="#666666" y_positions_count="11" y_param_info_id="$$param_id$1$.info" width="3" />
                <REPEAT count="$$direction$_channels_count$" index_name="channel_index" start="1">
                    <LAYER_STACK height="100%" id="$param_id$_meter.channel$channel_index$">
                        <CUS_VMETER param_id="$$param_id$$channel_index$$" height="100%"/>
                        <CUS_VMETER_HOLD param_id="$$hold_param_id$$channel_index$$" height="100%"/>
                    </LAYER_STACK>
                    <CELL width="4" display="false" id="$direction$_meter_spacer$channel_index$"/>
                    <PARAM_LINK from="main_bus_$direction$_delimiter_index" to="$direction$_meter_spacer$channel_index$.display" formula="(x==$channel_index$)"/>
                    <PARAM_LINK from="dsp.connected_$direction$s_count" to="$param_id$_meter.channel$channel_index$.display" formula="$channel_index$&lt;=x"/>
                </REPEAT>
                <GRID y_subdiv_count="1" height="100%" subdiv_color="$RULER_SUBDIV_COLOR$" origin_color="#666666" color="#666666" y_positions_count="11" y_param_info_id="$$param_id$1$.info" width="3"/>
            </ROW>
            <REPEAT count="$ruler_after$==1">
                <RULER id="$param_id$_meter.ruler" param_info_id="$$param_id$1$.info" positions_count="11" height="100%" text_v_align="center" v_margin="6" width="15" orientation="vertical" content="{value}" value_format=".0" text_color="$RULER_COLOR$" font_size="11"/>
            </REPEAT>
        </ROW>
    </TEMPLATE>

  <!-- Animations -->
  <TPL_SHOW_HIDE_ANIMATOR id="show_preferences_pane" object_id="preferences_pane" animated_param="v_offset" show_value="0" hide_value="-500" default="0"/>
  <TPL_SHOW_HIDE_ANIMATOR id="display_out_levels" object_id="out_levels" animated_param="v_offset" show_value="0" hide_value="500" default="1" duration_ms="300"/>
  <TPL_SHOW_HIDE_ANIMATOR id="display_in_levels" object_id="in_levels" animated_param="v_offset" show_value="0" hide_value="500" default="1" duration_ms="300"/>
  <PARAM_MULTI_LINK from="$show_levels$" to="display_in_levels;display_out_levels"/>

    <!-- Additional model data *****************************************************-->
    <FORMULA_PARAM id="main_bus_channels_count" formula="min(dsp.connected_inputs_count,dsp.connected_outputs_count)+((dsp.connected_inputs_count==1)and(dsp.connected_outputs_count==2))"/>

    <!-- Skin Layout starts here-->
  <THEME_DECORATIONS minimum_width="500">
    <THEME_TITLE_BAR text="Blue Cat's Plug'n Script">
      <!-- Meters -->
      <CELL width="7"/>
      <THEME_SHOW_METERS_BUTTON param_id="$show_levels$">
        <TOOLTIP text="Show Meters"/>
      </THEME_SHOW_METERS_BUTTON>
      <CUS_LAYOUT_BUTTON param_id="thin_display" reverse="true">
        <TOOLTIP text="Toggle Thin/Large Display"/>
      </CUS_LAYOUT_BUTTON>
      <!-- Script Commands -->
      <CELL width="7"/>
      <THEME_REFRESH_PARAM_BUTTON param_id="$reload_script$">
        <TOOLTIP text="Reload Script"/>
      </THEME_REFRESH_PARAM_BUTTON>
      <THEME_EDIT_ACTION_BUTTON action_id="OpenScriptFile" id="ëdit_script_button">
          <PARAM_LINK from="$script_file_type$" to="ëdit_script_button.enabled" formula="x==0"/>
        <TOOLTIP text="Edit Script"/>
      </THEME_EDIT_ACTION_BUTTON>
      <THEME_FOLDER_ACTION_BUTTON action_id="OpenScriptDirectory">
            <TOOLTIP text="Reveal Script in $FILE_EXPLORER_APP_NAME$"/>
      </THEME_FOLDER_ACTION_BUTTON>
      <CELL width="7"/>
      <IMAGE_PARAM_BUTTON param_id="show_preferences_pane" image="prefs_button.png" image_focus="prefs_button_hover.png" reverse="false" cursor="system::hand" ignore_mask_files="true">
        <TOOLTIP text="Show Preferences"/>
      </IMAGE_PARAM_BUTTON>
    </THEME_TITLE_BAR>
    <LAYER_STACK>
        <CELL width="500"/>
        <ROW spacing="$spacing$">
          <!-- In METERS -->
          <CUS_DARK_BOX layout_type="column" spacing="2" h_margin="10" id="in_levels" height="100%">
            <TPL_CHANNEL_METER param_id="input_level" hold_param_id="input_hold_level"/>
          </CUS_DARK_BOX>
          <COLUMN spacing="$spacing$">
            <!-- Minimum width cell -->
            <CELL width="$minimum_width$" height="-$spacing$"/>
              <!-- Script selection -->
              <ROW spacing="5">
                  <TEXT value="Select Script:"/>
                  <THEME_BOX layout_type="layer_stack">
                      <INVISIBLE_ACTION_BUTTON action_id="PlugNScript.ShowScriptsMenu" id="menu_button" cursor="system::hand" width="100%" height="100%"/>
                      <ROW>
                          <TEXT string_id="PlugNScript.script_name" width="100" id="menu_button_text"/>
                          <IMAGE_PARAM_METER param_id="menu_button.pushed" image="arrow_button.png" images_count="2"/>
                      </ROW>
                      <PARAM_LINK from="menu_button.pushed" to="menu_button_text.v_offset"/>
                      <PARAM_LINK from="menu_button.pushed" to="menu_button_text.h_offset"/>
                  </THEME_BOX>
              </ROW>
            <!--Script status-->
            <CUS_DARK_BOX spacing="8" width="100%" layout_type="column">
              <TEXT string_id="$script_name_string$" font_size="17" font_weight="bold" width="200"/>
              <TEXT string_id="$script_description_string$" width="100%"/>
            </CUS_DARK_BOX>

            <!-- NO Parameters box, if no script parameters available-->
            <FORMULA_PARAM id="no_controls_available" formula="($script_input_params_count$+$script_output_params_count$+$script_input_strings_count$)==0"/>
            <PARAM_LINK from="no_controls_available" to="no_controls_box.display"/>
            
            <THEME_BOX spacing="0" layout_type="column" id="no_controls_box" display="false" height="90">
                <CELL width="$minimum_width$"/>
                <TEXT value="This script has no controls."/>
            </THEME_BOX>
            
            <!-- Script Strings (Input Strings) -->
            <PARAM_LINK from="$script_input_strings_count$" to="in_strings_box.display" formula="x>0"/>
            <THEME_BOX layout_type="column" id="in_strings_box" display="false" spacing="$spacing$" width="100%">
              <REPEAT count="$SCRIPT_STRINGS_COUNT$">
                <PARAM_LINK from="$script_input_strings_count$" to="in_string_cell$index$.display" formula="(x>$index$)"/>
                <ROW id="in_string_cell$index$" spacing="$spacing$" display="false" width="100%" fit_text="abbreviate">
                  <TEXT string_id="$script_input_string$index$_name$" flex="1" height="100%" auto_size="false" text_h_align="left"/>
                  <TEXT value=":" height="17"/>
                  <TEXT_EDIT_BOX string_id="$script_input_string$index$$" background_color="#182f41" text_color="#ffffff" show_edges="false" flex="2" height="100%"/>
                </ROW> 
                </REPEAT>
            </THEME_BOX>

            <!-- Script Controls (Input Parameters) -->

            <!-- the first box determins the width, except for second box when in row mode (=0)-->
            <FORMULA_PARAM id="use_columns" formula="(thin_display)&amp;&amp;(($script_input_params_count$+$script_output_params_count$)&lt;=32)"/>
            <FORMULA_PARAM id="second_cell_width" formula="max((use_columns)*base_cell0.width,out_base_cell1.width)"/>
            <PARAM_LINK from="second_cell_width" to="adjustment_cell1.width" />
            <PARAM_LINK from="out_base_cell0.width" to="adjustment_cell0.width" />

            <REPEAT count="$SCRIPT_PARAMS_COUNT$/8" index_name="row_index">
              <PARAM_LINK from="$script_input_params_count$" to="in_box$row_index$.display" formula="x>$row_index$*8"/>
              <PARAM_LINK from="use_columns" to="in_box$row_index$.layout_type" formula="x==1"/>
              <CELL id="in_box$row_index$" layout_type="column" spacing="$spacing$" display="false">
                <REPEAT count="2" index_name="subgroup">
                  <LOCAL_VARIABLE id="group_box_index" formula="$row_index$*2+$subgroup$"/>
                  <PARAM_LINK from="$script_input_params_count$" to="in_group_box$group_box_index$.display" formula="x>$group_box_index$*4"/>
                  <IF condition="$group_box_index$>1">
                    <PARAM_LINK from="base_cell0.width" to="adjustment_cell$group_box_index$.width"/>
                  </IF>
                  <THEME_BOX spacing="0" layout_type="column" id="in_group_box$group_box_index$" display="false">
                    <IF condition="$group_box_index$==0">
                      <CELL width="$minimum_width$"/>
                    </IF>
                    <CELL id="adjustment_cell$group_box_index$"/>
                    <COLUMN>
                    <IF condition="$group_box_index$&lt;=1">
                      <CELL id="base_cell$group_box_index$" width="100%"/>
                    </IF>
                    <ROW spacing="16">
                      <REPEAT count="4" start="0">
                        <LOCAL_VARIABLE id="param_index" formula="$group_box_index$*4+$index$"/>
                        <PARAM_LINK from="$script_input_params_count$" to="input_col$param_index$.display" formula="(x>$param_index$)"/>
                        <COLUMN v_align="top" spacing="3" id="input_col$param_index$" display="false">
                          <PARAM_TEXT param_id="custom_param$param_index$" content="{name}"  width="100%" cursor="system::hand" font_weight="bold" fit_text="abbreviate"/>
                          <PARAM_TEXT param_id="custom_param$param_index$" width="65">
                            <INVISIBLE_PARAM_MENU_BUTTON width="100%" height="100%"  param_id="custom_param$param_index$" visible="false" id="input_control_menu$param_index$" cursor="system::hand"/>
                          </PARAM_TEXT>
                          <COLUMN spacing="-8">
                            <LAYER_STACK>
                              <THEME_V_SELECT_SWITCH  param_id="custom_param$param_index$" id="input_control_switch$param_index$" visible="false"/>
                              <CUS_INPUT_KNOB param_id="custom_param$param_index$" id="input_control$param_index$"/>
                            </LAYER_STACK>
                            <CUS_INPUT_DSP_SETTINGS_BUTTON param_id="$script_input$param_index$$"/>
                          </COLUMN>
                        </COLUMN>
                      </REPEAT>
                    </ROW>
                    </COLUMN>
                  </THEME_BOX>
                </REPEAT>
              </CELL>
            </REPEAT>

            <!-- Script Outputs -->
            <FORMULA_PARAM id="out_second_cell_width" formula="max((use_columns)*out_base_cell0.width,base_cell1.width)"/>
            <PARAM_LINK from="out_second_cell_width" to="out_adjustment_cell1.width"/>
            <PARAM_LINK from="base_cell0.width" to="out_adjustment_cell0.width" />

            <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$/8" index_name="row_index">
              <PARAM_LINK from="$script_output_params_count$" to="out_box$row_index$.display" formula="x>$row_index$*8"/>
              <PARAM_LINK from="use_columns" to="out_box$row_index$.layout_type" formula="x==1"/>
              <CELL id="out_box$row_index$" layout_type="column" spacing="$spacing$" display="false">
                <REPEAT count="2" index_name="subgroup">
                  <LOCAL_VARIABLE id="group_box_index" formula="$row_index$*2+$subgroup$"/>
                  <PARAM_LINK from="$script_output_params_count$" to="out_group_box$group_box_index$.display" formula="x>$group_box_index$*4"/>
                  <IF condition="$group_box_index$>1">
                    <PARAM_LINK from="out_base_cell0.width" to="out_adjustment_cell$group_box_index$.width"/>
                  </IF>
                  <THEME_BOX spacing="0" layout_type="column" id="out_group_box$group_box_index$" display="false">
                    <IF condition="$group_box_index$==0">
                      <CELL width="$minimum_width$"/>
                    </IF>
                    <CELL id="out_adjustment_cell$group_box_index$"/>
                    <COLUMN>
                      <IF condition="$group_box_index$&lt;=1">
                        <CELL id="out_base_cell$group_box_index$" width="100%"/>
                      </IF>
                      <ROW spacing="16">
                      <REPEAT count="4" start="0">
                        <LOCAL_VARIABLE id="param_index" formula="$group_box_index$*4+$index$"/>
                        <PARAM_LINK from="$script_output_params_count$" to="out_col$param_index$.display" formula="(x>$param_index$)"/>
                        <COLUMN v_align="top" spacing="3" id="out_col$param_index$" display="false">
                          <PARAM_TEXT param_id="custom_out_param$param_index$" content="{name}"  width="100%" cursor="system::hand" font_weight="bold" fit_text="abbreviate"/>
                          <PARAM_TEXT param_id="custom_out_param$param_index$" width="65"/>
                          <COLUMN spacing="-2">
                            <CUS_OUTPUT_METER param_id="custom_out_param$param_index$" id="out_control$param_index$"/>
                            <CUS_OUTPUT_DSP_SETTINGS_BUTTON param_id="$script_output$param_index$$"/>
                          </COLUMN>
                        </COLUMN>
                      </REPEAT>
                    </ROW>
                    </COLUMN>
                  </THEME_BOX>
                </REPEAT>
              </CELL>
            </REPEAT>

            <!-- Messages and status -->
            <THEME_BOX spacing="0" layout_type="layer_stack" width="100%" h_margin="4">
              <INVISIBLE_ACTION_BUTTON width="100%" height="100%" action="OpenLogFile" cursor="system::hand" id="open_log_button">
                    <TOOLTIP text="Click to open full log"/>
              </INVISIBLE_ACTION_BUTTON>
              <ROW width="100%" spacing="5" id="open_log_button_content">
                <IMAGE_PARAM_METER param_id="$script_status$" image="status16.png" images_count="4"/>
                <TEXT string_id="$script_status_string$" flex="1" text_h_align="left" fit_text="abbreviate"/>
                <TEXT value="..."/>
              </ROW>
                <PARAM_LINK from="open_log_button.pushed" to="open_log_button_content.v_offset"/>
                <PARAM_LINK from="open_log_button.pushed" to="open_log_button_content.h_offset"/>
            </THEME_BOX>
          </COLUMN>
          <CUS_DARK_BOX layout_type="column" spacing="2" h_margin="10" height="100%" id="out_levels">
            <TPL_CHANNEL_METER param_id="output_level" hold_param_id="output_hold_level" ruler_after="1" direction="output"/>
          </CUS_DARK_BOX>
        </ROW>

      <!-- PREFERENCES PANE -->
      <THEME_BOX id="preferences_pane" display="false" layout_type="column" font_size="+2" opacity="80%" mouse_sensitive="true">
        <TEXT value="PREFERENCES" font_size="+1"/>
        <THEME_BOX_H_SEPARATOR/>
        <TEXT value="Edit script files with:" h_align="left" font_weight="bold"/>
        <ROW>
          <TEXT string_id="global.edit_script_application" width="300" text_h_align="left">
            <INVISIBLE_ACTION_BUTTON action_id="choose_edit_script_application" width="100%" height="100%" cursor="system::hand"/>
          </TEXT>
          <LAYER_STACK>
            <THEME_BOX v_margin="2">
              <TEXT value="..."/>
            </THEME_BOX>
            <ACTION id="choose_edit_script_application" type="DisplayFileOpenDialog" file_path_string_id="global.edit_script_application"  open_bundles="false"/>
            <INVISIBLE_ACTION_BUTTON action_id="choose_edit_script_application" flex="1" cursor="system::hand">
              <TOOLTIP text="Click to select an application"/>
            </INVISIBLE_ACTION_BUTTON>
          </LAYER_STACK>
        </ROW>
        <TEXT value="Open log files with:" h_align="left" font_weight="bold"/>
        <ROW>
          <TEXT string_id="global.open_log_application" width="300" text_h_align="left">
            <INVISIBLE_ACTION_BUTTON action_id="choose_open_log_application" width="100%" height="100%" cursor="system::hand"/>
          </TEXT>
          <LAYER_STACK>
            <THEME_BOX v_margin="2">
              <TEXT value="..."/>
            </THEME_BOX>
            <ACTION id="choose_open_log_application" type="DisplayFileOpenDialog" file_path_string_id="global.open_log_application" open_bundles="false"/>
            <INVISIBLE_ACTION_BUTTON action_id="choose_open_log_application" flex="1" cursor="system::hand">
              <TOOLTIP text="Click to select an application"/>
            </INVISIBLE_ACTION_BUTTON>
          </LAYER_STACK>
        </ROW>
        <THEME_BOX_H_SEPARATOR/>
        <THEME_BOX h_align="right">
          <TEXT value="Close">
            <INVISIBLE_PARAM_BUTTON param_id="show_preferences_pane" width="100%" height="100%" cursor="system::hand"/>
          </TEXT>
        </THEME_BOX>
      </THEME_BOX>
    </LAYER_STACK>
  </THEME_DECORATIONS>
</DUMMY>