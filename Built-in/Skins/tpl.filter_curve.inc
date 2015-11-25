<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
    <!-- log scale filter curve-->
    <TEMPLATE id="TPL_LOG_FILTER_CURVE">
        <!-- values usde in formula and set by script -->
        <PARAM id="$id$.kb0" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.kb1" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.kb2" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.ka0" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.ka1" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.ka2" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <!-- the formula -->
        <FORMULA_CURVE id="$id$" formula="10*log(($id$.kb0+$id$.kb1*cos(x)+$id$.kb2*cos(2*x))/($id$.ka0+$id$.ka1*cos(x)+$id$.ka2*cos(2*x)))/log(10)" y_min="-20" y_max="20" x_min="0.0028" x_max="2.85"/>
        <!-- script that defines the functions used to set curve values -->
        <SCRIPT src="tpl.filter_curve.cxx" preprocess="true"/>
    </TEMPLATE>

    <!-- linear scale filter curve-->
    <TEMPLATE id="TPL_LINEAR_FILTER_CURVE">
        <!-- values usde in formula and set by script -->
        <PARAM id="$id$.kb0" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.kb1" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.kb2" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.ka0" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.ka1" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <PARAM id="$id$.ka2" min="-$_PARAM_MAX_$" max="$_PARAM_MAX_$" exposed="true" />
        <!-- the formula -->
        <FORMULA_CURVE id="$id$" formula="sqrt(($id$.kb0+$id$.kb1*cos(x)+$id$.kb2*cos(2*x))/($id$.ka0+$id$.ka1*cos(x)+$id$.ka2*cos(2*x)))" y_min="0" y_max="1" x_min="0.0028" x_max="2.85"/>
        <!-- script that defines the functions used to set curve values -->
        <SCRIPT src="tpl.filter_curve.cxx" preprocess="true"/>
    </TEMPLATE>
</DUMMY>