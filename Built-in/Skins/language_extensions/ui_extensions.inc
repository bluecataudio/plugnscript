<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
  <!-- Simple Extensions to the Kitty Language for the UI layer -->

  <!-- Animator frozen during user interface load -->
  <TEMPLATE id="TPL_PARAM_ANIMATOR" persistent="false" duration_ms="250">
    <PARAM_ANIMATOR id="$id$" param_id="$param_id$" enabled="false" persistent="$persistent$" duration_ms="$duration_ms$"/>
    <PARAM_LINK from="window.loaded" to="$id$.enabled"/>
  </TEMPLATE>

  <!-- An animated parameter used to animate show/hide of a UI element -->
  <TEMPLATE id="TPL_SHOW_HIDE_ANIMATOR" object_id="#undefined control_id#" animated_param="opacity" show_param="display" show_value="1" hide_value="0" default="1" persistent="false" disable_on_ui_load="true" formula="x" duration_ms="250" refresh_time_ms="50">
    
    <!-- The animator should be disabled by default if enabled by the UI-->
    <LOCAL_VARIABLE id="enabled_by_default" value="true"/>
    <REPEAT count="$disable_on_ui_load$==true">
         <VARIABLE id="enabled_by_default" value="false" override="true"/>
    </REPEAT>    
      
    <!-- The animator -->
    <PARAM_ANIMATOR id="$id$" param_id="$id$.show" persistent="$persistent$" duration_ms="$duration_ms$" refresh_time_ms="$refresh_time_ms$" enabled="$enabled_by_default$"/>
    <PARAM_LINK from="window.loaded" to="$id$.enabled" enabled="$disable_on_ui_load$"/>

    <!-- The show/hide param -->
    <PARAM id="$id$.show" min="0" max="1" default="$default$" type="boolean"/>

    <!-- Define the actual parameter based on min/max show/hide values and link to the target object parameter-->
    <REPEAT count="$hide_value$&lt;$show_value$">
      <PARAM id="$id$.param" min="$hide_value$" max="$show_value$" default="$default_value$"/>
      <PARAM_LINK from="$id$.show" to="$id$.param" normalized="true" formula="$formula$"/>
    </REPEAT>
    <REPEAT count="$hide_value$&gt;=$show_value$">
      <PARAM id="$id$.param" min="$show_value$" max="$hide_value$" default="$default_value$"/>
      <PARAM_LINK from="$id$.show" to="$id$.param" normalized="true" reverse="true" formula="$formula$"/>
    </REPEAT>
    <PARAM_LINK from="$id$.param" to="$object_id$.$animated_param$"/>
    <!-- When hide value reached, hide the object-->
    <CONST_PARAM id="$id$.hide_value" value="$hide_value$"/>
    <PARAM_MULTI_LINK from="$id$.param;$id$.hide_value" to="$object_id$.$show_param$" mux_function="=" reverse="true"/>
  </TEMPLATE>

</DUMMY>