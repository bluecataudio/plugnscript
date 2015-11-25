<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
    <!-- Includes -->
    <INCLUDE_ONCE file="DarkBlue-Theme/theme.inc"/>
    <INCLUDE_ONCE file="vmeter/vmeter.inc"/>
    <INCLUDE_ONCE file="dark_box/box.inc"/>
    <INCLUDE_ONCE file="mapping.inc"/>
    <TEMPLATE id="TPL_MAIN_LAYOUT" layout_button="false" title="Blue Cat's Plug'n Script">
        <INCLUDE_ONCE file="language_extensions/language_extensions.inc"/>
        <INCLUDE_ONCE file="mapping.inc"/>
        <DEFINE>
            <CUS_LAYOUT_BUTTON base_type="IMAGE_PARAM_BUTTON" image="screen_layout_button.png" image_focus="screen_layout_button_hover.png" cursor="system::hand" images_count="2"/>
            <CUS_QMARK_ACTION_BUTTON  base_type="IMAGE_ACTION_BUTTON" image="q_mark_button.png" image_focus="q_mark_button_hover.png" cursor="system::hand"/>
        </DEFINE>
        <VARIABLE id="minimum_width" value="300"/>
        <VARIABLE id="minimum_height" value="150"/>
        <VARIABLE id="spacing" value="4"/>
        
        <VARIABLE id="RULER_SUBDIV_COLOR" value="#333333"/>
        <VARIABLE id="RULER_COLOR" value="#777777"/>
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
        <TPL_SHOW_HIDE_ANIMATOR id="show_preferences_pane" object_id="preferences_pane" animated_param="v_offset" show_value="0" hide_value="-500" default="-500"/>
        <TPL_SHOW_HIDE_ANIMATOR id="display_out_levels" object_id="out_levels" animated_param="v_offset" show_value="0" hide_value="500" default="1" duration_ms="300"/>
        <TPL_SHOW_HIDE_ANIMATOR id="display_in_levels" object_id="in_levels" animated_param="v_offset" show_value="0" hide_value="500" default="1" duration_ms="300"/>
        <PARAM_MULTI_LINK from="$show_levels$" to="display_in_levels;display_out_levels"/>
        
        <!-- Skin Layout starts here-->
        <THEME_DECORATIONS minimum_width="525" preset_box_width="180">
            <THEME_TITLE_BAR text="$title$">
                <!-- Meters -->
                <CELL width="7"/>
                <THEME_SHOW_METERS_BUTTON param_id="$show_levels$">
                    <TOOLTIP text="Show Meters"/>
                </THEME_SHOW_METERS_BUTTON>
                <CUS_LAYOUT_BUTTON param_id="thin_display" reverse="true" display="$layout_button$">
                    <TOOLTIP text="Toggle Thin/Large Display"/>
                </CUS_LAYOUT_BUTTON>
                <!-- Script Commands -->
                <CELL width="7"/>
                <THEME_REFRESH_PARAM_BUTTON param_id="$reload_script$">
                    <TOOLTIP text="Reload Script"/>
                </THEME_REFRESH_PARAM_BUTTON>
                <THEME_EDIT_ACTION_BUTTON action_id="OpenScriptFile" id="edit_script_button">
                    <PARAM_LINK from="$script_file_type$" to="edit_script_button.enabled" formula="x==0"/>
                    <TOOLTIP text="Edit Script"/>
                </THEME_EDIT_ACTION_BUTTON>
                <THEME_FOLDER_ACTION_BUTTON action_id="OpenScriptDirectory">
                    <TOOLTIP text="Reveal Script in $FILE_EXPLORER_APP_NAME$"/>
                </THEME_FOLDER_ACTION_BUTTON>
                <CUS_QMARK_ACTION_BUTTON action_id="OpenScriptDoc" display="false" id="open_script_doc_button">
                    <PARAM_LINK from="$script_doc_path$.length" to="open_script_doc_button.display" formula="x!=0"/>
                    <TOOLTIP text="Display Script User Manual"/>
                </CUS_QMARK_ACTION_BUTTON>
                <CELL width="7"/>
                <IMAGE_PARAM_BUTTON param_id="show_preferences_pane" image="prefs_button.png" image_focus="prefs_button_hover.png" reverse="false" cursor="system::hand" ignore_mask_files="true">
                    <TOOLTIP text="Show Preferences"/>
                </IMAGE_PARAM_BUTTON>
            </THEME_TITLE_BAR>
            <LAYER_STACK>
                <CELL width="500" />
                <ROW spacing="$spacing$">
                    <CELL height="$minimum_height$" width="-$spacing$"/>
                    <!-- In METERS -->
                    <CUS_DARK_BOX layout_type="column" spacing="2" h_margin="10" id="in_levels" height="100%">
                        <TPL_CHANNEL_METER param_id="input_level" hold_param_id="input_hold_level"/>
                    </CUS_DARK_BOX>
                    <COLUMN spacing="$spacing$" v_align="bottom">
                        <!-- Minimum width cell -->
                        <CELL width="$minimum_width$" height="-$spacing$"/>
                        
                        <!-- custom content goes here -->
                        <TEMPLATE_INNER_CONTENT/>
                        
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
                <WIDGET system_widget="true" id="preferences_pane" display="false" >
                    <THEME_BOX layout_type="column" font_size="+2" opacity="80%" mouse_sensitive="true">
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
                </WIDGET>
            </LAYER_STACK>
        </THEME_DECORATIONS>
    </TEMPLATE>
</DUMMY>
