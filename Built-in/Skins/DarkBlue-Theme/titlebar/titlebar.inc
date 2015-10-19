<?xml version="1.0" encoding="utf-8" ?>
<TEMPLATE id="TPL_TITLE_BAR" text="$PLUGIN_NAME$" text_string_id="" text_width="-1">
	<LAYER_STACK h_align="right" width="100%" >
        <!-- Background image -->
		<IMAGE_GROUP_BOX image="$THEME_PATH$/titlebar/title_bg_long.png" h_margin="2" h_align="right" stretch="true" width="100%" height="22"/>
		<ROW h_align="right" spacing="4" h_margin="2">
            <TEXT text_color="#eeeeee" value="$text$" string_id="$text_string_id$" h_align="right" font_size="19" font_face="Verdana" font_style="italic" id="title_bar.text" width="$text_width$" text_h_align="right"/>
            <IMAGE image="$THEME_PATH$/titlebar/logo.png"/>
		</ROW>
		<ROW h_align="left">
            <!-- Bypass -->
            <IMAGE_PARAM_BUTTON param_id="dsp.input0" image="$THEME_PATH$/titlebar/power_button.png" image_hover="$THEME_PATH$/titlebar/power_button_hi.png"  image_focus="$THEME_PATH$/titlebar/power_button_hi.png" reverse="true" cursor="system::hand">
                <TOOLTIP text="Bypass"/>
            </IMAGE_PARAM_BUTTON>
            <!-- opacity -->
            <PARAM id="theme.opacity" min="0" max="100" default="100" unit="%" persistent="true" name="Opacity"/>
            <PARAM_LINK from="theme.opacity" to="window.opacity" normalized="true"/>
            <THEME_MINI_KNOB param_id="theme.opacity" positions_count="101">
                <PARAM_TOOLTIP param_id="theme.opacity" value_format=".0" content="Opacity: {value}{unit}" show_on_click="true"/>
            </THEME_MINI_KNOB>
            <PARAM id="theme.show_settings" type="boolean" default="0" persistent="true"/>
            <IMAGE_PARAM_BUTTON param_id="theme.show_settings" image="$THEME_PATH$/titlebar/gears_button.png" image_focus="$THEME_PATH$/titlebar/gears_button_hover.png" reverse="false" cursor="system::hand">
                <TOOLTIP text="Show Controls Settings"/>
            </IMAGE_PARAM_BUTTON>
            <TEMPLATE_INNER_CONTENT/>
        </ROW>
	</LAYER_STACK>
</TEMPLATE>
