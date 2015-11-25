<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
    <!-- Main layout around cntrols is defined as a template for reuse with other skins -->
    <INCLUDE_ONCE file="tpl.main_layout.inc"/>
    <TPL_MAIN_LAYOUT layout_button="true">
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
            <TEXT value="NATIVE" id="script_is_bin_text" text_color="#999999">
                <TOOLTIP text="this is a (compiled) binary script that cannot be edited directly in the plug-in"/>
            </TEXT>
            <PARAM_LINK from="$script_file_type$" to="script_is_bin_text.visible"/>
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
        <!-- using custom parameters info to configure our controls -->
        <REPEAT count="$SCRIPT_PARAMS_COUNT$">
            <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control$index$.positions_count"/>
            <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control_menu$index$.positions_count"/>
            <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control$index$.visible" formula="x!=2"/>
            <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control_switch$index$.visible" formula="x==2"/>
            <PARAM_LINK from="$script_input_param$index$_steps$" to="input_control_menu$index$.visible" formula="(x>=2)and(x&lt;=50)"/>
        </REPEAT>
        
        <!-- the first box determines the width, except for second box when in row mode (=0)-->
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
    </TPL_MAIN_LAYOUT>
</DUMMY>