<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
    <!-- General variables *****************************************************-->
    <VARIABLE id="input_channels_count" value="$PLUGIN_AUDIO_INPUTS_COUNT$"/>
    <VARIABLE id="output_channels_count" value="$PLUGIN_AUDIO_OUTPUTS_COUNT$"/>
    <VARIABLE id="SCRIPT_PARAMS_COUNT" value="48"/>
    <VARIABLE id="SCRIPT_STRINGS_COUNT" value="16"/>
    <VARIABLE id="SCRIPT_OUT_PARAMS_COUNT" value="32"/>

    <!-- Basic definitions based on channels/bands count ***********************-->
    <DEFINE>
        <FOR_EACH_IN_CHANNEL base_type="REPEAT" count="$input_channels_count$" index_name="channel_index" start="1"/>
        <FOR_EACH_OUT_CHANNEL base_type="REPEAT" count="$output_channels_count$" index_name="channel_index" start="1"/>
    </DEFINE>

    <!-- The mapping utilities that define the NEXT_ / CURRENT_ macros-->
    <INCLUDE_ONCE file="mapping_utils.inc"/>

    <!-- Input Parameters ******************************************************-->
    <NEXT_INPUT_PARAM id="bypass"/>
    <NEXT_INPUT_PARAM id="reload_script"/>
    <NEXT_INPUT_PARAM id="show_levels"/>
    <NEXT_INPUT_PARAM id="reserved3"/>
    <NEXT_INPUT_PARAM id="reserved4"/>
    <NEXT_INPUT_PARAM id="reserved5"/>
    <NEXT_INPUT_PARAM id="reserved6"/>
    <NEXT_INPUT_PARAM id="reserved7"/>
    <REPEAT count="$SCRIPT_PARAMS_COUNT$">
        <NEXT_INPUT_PARAM id="script_input$index$"/>
    </REPEAT>

    <!-- Input Strings ******************************************************-->
    <NEXT_INPUT_STRING id="script_file_selection"/>
    <REPEAT count="$SCRIPT_STRINGS_COUNT$">
        <NEXT_INPUT_STRING id="script_input_string$index$"/>
    </REPEAT>

    <!-- Private Output Parameters ******************************************************-->
    <NEXT_PRIVATE_OUTPUT_PARAM id="script_status"/>
    <NEXT_PRIVATE_OUTPUT_PARAM id="script_input_params_count"/>
    <NEXT_PRIVATE_OUTPUT_PARAM id="script_output_params_count"/>
    <REPEAT count="$SCRIPT_PARAMS_COUNT$">
        <NEXT_PRIVATE_OUTPUT_PARAM id="script_input_param$index$_min"/>
        <NEXT_PRIVATE_OUTPUT_PARAM id="script_input_param$index$_max"/>
        <NEXT_PRIVATE_OUTPUT_PARAM id="script_input_param$index$_default"/>
        <NEXT_PRIVATE_OUTPUT_PARAM id="script_input_param$index$_steps"/>
    </REPEAT>
    <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$">
        <NEXT_PRIVATE_OUTPUT_PARAM id="script_output_param$index$_min"/>
        <NEXT_PRIVATE_OUTPUT_PARAM id="script_output_param$index$_max"/>
        <NEXT_PRIVATE_OUTPUT_PARAM id="script_output_param$index$_default"/>
    </REPEAT>
    <FOR_EACH_IN_CHANNEL>
        <NEXT_PRIVATE_OUTPUT_PARAM id="input_level$channel_index$"/>
        <NEXT_PRIVATE_OUTPUT_PARAM id="input_hold_level$channel_index$"/>
    </FOR_EACH_IN_CHANNEL>
    <FOR_EACH_OUT_CHANNEL>
        <NEXT_PRIVATE_OUTPUT_PARAM id="output_level$channel_index$"/>
        <NEXT_PRIVATE_OUTPUT_PARAM id="output_hold_level$channel_index$"/>
    </FOR_EACH_OUT_CHANNEL>
    <NEXT_PRIVATE_OUTPUT_PARAM id="script_input_strings_count"/>
    <NEXT_PRIVATE_OUTPUT_PARAM id="script_file_type"/>

    <!-- Output Parameters ******************************************************-->
    <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$">
        <NEXT_OUTPUT_PARAM id="script_output$index$"/>
    </REPEAT>

    <!-- Output Strings ******************************************************-->
    <NEXT_OUTPUT_STRING id="script_status_string"/>
    <NEXT_OUTPUT_STRING id="script_name_string"/>
    <NEXT_OUTPUT_STRING id="script_description_string"/>
    <NEXT_OUTPUT_STRING id="script_file_path"/>
    <NEXT_OUTPUT_STRING id="log_file_path"/>
    <NEXT_OUTPUT_STRING id="script_doc_path"/>
    <NEXT_OUTPUT_STRING id="reserved_string6"/>
    <NEXT_OUTPUT_STRING id="reserved_string7"/>

    <REPEAT count="$SCRIPT_PARAMS_COUNT$">
        <NEXT_OUTPUT_STRING id="script_input_param$index$_name"/>
        <NEXT_OUTPUT_STRING id="script_input_param$index$_unit"/>
        <NEXT_OUTPUT_STRING id="script_input_param$index$_enum_values"/>
    </REPEAT>
    <REPEAT count="$SCRIPT_OUT_PARAMS_COUNT$">
        <NEXT_OUTPUT_STRING id="script_output_param$index$_name"/>
        <NEXT_OUTPUT_STRING id="script_output_param$index$_unit"/>
        <NEXT_OUTPUT_STRING id="script_output_param$index$_enum_values"/>
    </REPEAT>
    <REPEAT count="$SCRIPT_STRINGS_COUNT$">
        <NEXT_OUTPUT_STRING id="script_input_string$index$_name"/>
    </REPEAT>
    <!-- Extra params and variables *********************************************-->
    <!-- Opacity -->
    <PARAM id="global_opacity" min="0" max="100" default="100" unit="%" persistent="true"/>
    <PARAM_LINK from="global_opacity" to="window.opacity" normalized="true"/>

    <!-- Main audio bus channels-->
    <FORMULA_PARAM id="main_bus_channels_count" formula="min(dsp.connected_inputs_count,dsp.connected_outputs_count)"/>

    <!-- Custom parameters, built from param details in current script -->
    <REPEAT count="$SCRIPT_PARAMS_COUNT$">
        <!-- Input param for display and control, linked to actual DSP param-->
        <PARAM id="custom_param$index$" min="0" max="1" default="0" name="Param $index$"/>
        <PARAM_LINK from="$script_input_param$index$_min$" to="custom_param$index$.min"/>
        <PARAM_LINK from="$script_input_param$index$_max$" to="custom_param$index$.max"/>
        <PARAM_LINK from="$script_input_param$index$_default$" to="custom_param$index$.default"/>
        <STRING_LINK from="$script_input_param$index$_unit$" to="custom_param$index$.unit"/>
        <STRING_LINK from="$script_input_param$index$_name$" to="custom_param$index$.name"/>
        <STRING_LINK from="$script_input_param$index$_enum_values$" to="custom_param$index$.enum_values"/>

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

    <!-- Custom actions-->
    <INCLUDE_ONCE file="$_SYSTEM_TYPE_$.inc"/>
    <STRING id="PlugNScript.script_name" default="No Script"/>
    <ACTION id="PlugNScript.ShowScriptsMenu" type="DisplayPlugNScriptMenu" script_file_name_string_id="PlugNScript.script_name"/>
    <ACTION id="OpenScriptFile" type="OpenUrl" url_string_id="$script_file_path$" application_string_id="global.edit_script_application"/>
    <ACTION id="OpenScriptDoc" type="OpenUrl" url_string_id="$script_doc_path$"/>
    <ACTION id="OpenLogFile" type="OpenUrl" url_string_id="$log_file_path$"  application_string_id="global.open_log_application"/>
    <ACTION id="OpenScriptDirectory" type="OpenUrl" url_string_id="$script_file_path$" application="$FILE_EXPLORER_APP$"/>
</DUMMY>