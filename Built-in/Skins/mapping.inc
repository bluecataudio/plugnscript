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
  <NEXT_OUTPUT_STRING id="reserved_string5"/>
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
</DUMMY>