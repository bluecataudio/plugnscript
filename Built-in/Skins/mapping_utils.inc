<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
  <TEMPLATE id="TPL_CURRENT_VALUE" prefix="#UNDEFINED#" variable="#UNDEFINED#">
    <VARIABLE id="$id$" value="$prefix$$$variable$$"/>
  </TEMPLATE>
  
  <TEMPLATE id="TPL_NEXT_VALUE" prefix="#UNDEFINED#" variable="#UNDEFINED#">
    <VARIABLE id="$variable$" formula="$$variable$$+1" override="true"/>
    <TPL_CURRENT_VALUE id="$id$" prefix="$prefix$" variable="$variable$"/>
  </TEMPLATE>
  
  <VARIABLE id="current_input_param_index" value="-1"/>
  <VARIABLE id="current_input_string_index" value="-1"/>
  <VARIABLE id="current_output_param_index" value="-1"/>
  <VARIABLE id="current_private_output_param_index" value="-1"/>
  <VARIABLE id="current_output_curve_index" value="-1"/>
  <VARIABLE id="current_output_string_index" value="-1"/>
  <DEFINE>
    <NEXT_INPUT_PARAM base_type="TPL_NEXT_VALUE" prefix="dsp.input" variable="current_input_param_index"/>
    <NEXT_INPUT_STRING base_type="TPL_NEXT_VALUE" prefix="dsp.input_string" variable="current_input_string_index"/>
    <NEXT_OUTPUT_PARAM base_type="TPL_NEXT_VALUE" prefix="dsp.output" variable="current_output_param_index"/>
    <NEXT_PRIVATE_OUTPUT_PARAM base_type="TPL_NEXT_VALUE" prefix="dsp.private_output" variable="current_private_output_param_index"/>
    <NEXT_OUTPUT_CURVE base_type="TPL_NEXT_VALUE" prefix="dsp.output_curve" variable="current_output_curve_index"/>
    <NEXT_OUTPUT_STRING base_type="TPL_NEXT_VALUE" prefix="dsp.output_string" variable="current_output_string_index"/>
    <CURRENT_INPUT_PARAM base_type="TPL_CURRENT_VALUE" prefix="dsp.input" variable="current_input_param_index"/>
    <CURRENT_INPUT_STRING base_type="TPL_CURRENT_VALUE" prefix="dsp.input_string" variable="current_input_string_index"/>
    <CURRENT_OUTPUT_PARAM base_type="TPL_CURRENT_VALUE" prefix="dsp.output" variable="current_output_param_index"/>
    <CURRENT_PRIVATE_OUTPUT_PARAM base_type="TPL_CURRENT_VALUE" prefix="dsp.private_output" variable="current_private_output_param_index"/>
    <CURRENT_OUTPUT_CURVE base_type="TPL_CURRENT_VALUE" prefix="dsp.output_curve" variable="current_output_curve_index"/>
    <CURRENT_OUTPUT_STRING base_type="TPL_CURRENT_VALUE" prefix="dsp.output_string" variable="current_output_string_index"/>
  </DEFINE>
</DUMMY>