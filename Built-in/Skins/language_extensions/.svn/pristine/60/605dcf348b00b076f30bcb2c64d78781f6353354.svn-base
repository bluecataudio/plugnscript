<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
  <!-- Simple Extensions to the Kitty Language for the model layer -->
  
  <!-- Constant value parameter: -->
  <!-- Parameters: value -->
  <TEMPLATE id="CONST_PARAM">
      <PARAM id="$id$" default="$value$" min="$value$" max="$value$"/>
  </TEMPLATE>
  
  <!-- exclusive link template for booleans -->
  <TEMPLATE id="PARAM_MULTI_EXCLUSIVE_LINK" param_ids="#undefined params_ids#">
    <PARAM_MULTI_LINK from="$param_ids$" to="$param_ids$" trigger_min="1" reverse="true" mux_function="max"/>
  </TEMPLATE>
    
  <!-- build time IF -->
  <TEMPLATE id="IF" condition="0">
      <REPEAT count="($condition$)!=0" index_name="_if_index_">
          <TEMPLATE_INNER_CONTENT/>
      </REPEAT>
  </TEMPLATE>
</DUMMY>