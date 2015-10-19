<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
	<!-- toolbar item template -->
	<TEMPLATE id="TPL_TOOLBAR_ITEM" short_text="">
    <ROW h_align="$align$" spacing="3" >
      <IMAGE_ACTION_BUTTON image="$image$" image_mouse_mask="16x16_mouse_mask.png"  image_pushed_mouse_mask="16x16_mouse_mask.png" image_hover_mouse_mask="16x16_mouse_mask.png" cursor="system::hand" action="$action$" opacity="$opacity$" auto_focus="false">
        <TOOLTIP text="$text$"/>
      </IMAGE_ACTION_BUTTON>
      <TEXT value="$short_text$">
        <INVISIBLE_ACTION_BUTTON cursor="system::hand" action="$action$" width="100%" height="100%">
          <TOOLTIP text="$text$"/>
        </INVISIBLE_ACTION_BUTTON>
      </TEXT>
    </ROW>
	</TEMPLATE>
	
	<!-- colored toolbar button template (with animation) -->
	<TEMPLATE id="TPL_TOOLBAR_BUTTON" button_id="#UNDEFINED#">
      <IMAGE_GROUP_BOX image="$THEME_PATH$/toolbar/black_box.png" v_margin="1" h_margin="1" height="100%">
        <PARAM_LINK from="$button_id$.mouse_over" to="$button_id$.opacity_animator" normalized="true"/>
        <PARAM_ANIMATOR id="$button_id$.opacity_animator" param_id="$button_id$.color_bg.opacity" duration_ms="200"/>
        <LAYER_STACK height="100%">
            <IMAGE  width="100%" height="100%" image="$THEME_PATH$/toolbar/title_bg.png" repeat="true" id="$button_id$.color_bg" visible="true" opacity="0%"/>
            <TEMPLATE_INNER_CONTENT/>
         </LAYER_STACK>
      </IMAGE_GROUP_BOX>
	</TEMPLATE>
	
	<!-- The actual horizontal toolbar template -->
	<TEMPLATE id="TPL_TOOLBAR" icon_opacity="100%" bar_opacity="100%" width="100%" font_size="13" preset_box_width="120">
	<ROW width="$width$" font_size="$font_size$">
    <!-- toolbar bacground -->
		<IMAGE width="100%" h_margin="1" image="$THEME_PATH$/toolbar/toolbar_bg.png" repeat="true" spacing="5" layout_type="row" height="18" opacity="$bar_opacity$">
      <!-- Main menu -->
      <TPL_TOOLBAR_ITEM image="$THEME_PATH$/toolbar/menu.png" action="OpenMenu" text="Plug-in Menu" short_text="Menu" opacity="$icon_opacity$" h_align="left"/>
      <CELL flex="1"/>
      <!-- Undo/Redo -->
			<ROW spacing="5">
        <TPL_TOOLBAR_ITEM image="$THEME_PATH$/toolbar/undo.png" action="Undo" text="Undo" short_text="Undo" opacity="$icon_opacity$"/>
        <TPL_TOOLBAR_ITEM image="$THEME_PATH$/toolbar/redo.png" action="Redo" text="Redo" short_text="Redo" opacity="$icon_opacity$"/>
			</ROW>
      <CELL flex=".05"/>
      <!-- Presets area -->
      <ROW font_size="+1" height="100%">
        <TPL_TOOLBAR_BUTTON button_id="prev_preset_button">
          <LAYER_STACK h_margin="3" height="100%">
              <IMAGE image="$THEME_PATH$/toolbar/arrow_left.png" opacity="70%"/>
           </LAYER_STACK>
          <INVISIBLE_ACTION_BUTTON action="PreviousPreset" width="100%" height="100%" cursor="system::hand" id="prev_preset_button">
              <TOOLTIP text="Previous Preset"/>
          </INVISIBLE_ACTION_BUTTON>
       </TPL_TOOLBAR_BUTTON>

        <TPL_TOOLBAR_BUTTON button_id="presets_button">
          <TEXT string_id="theme.toolbar.preset_display_name" width="$preset_box_width$"/>
          <INVISIBLE_ACTION_BUTTON action="OpenPresetsMenu" width="100%" height="100%" cursor="system::hand" id="presets_button">
              <TOOLTIP text="Presets Menu"/>
          </INVISIBLE_ACTION_BUTTON>        
        </TPL_TOOLBAR_BUTTON>
        
        <TPL_TOOLBAR_BUTTON button_id="next_preset_button">
          <LAYER_STACK h_margin="3" height="100%">
              <IMAGE image="$THEME_PATH$/toolbar/arrow_right.png" opacity="70%"/>
          </LAYER_STACK>
          <INVISIBLE_ACTION_BUTTON action="NextPreset" width="100%" height="100%" cursor="system::hand" id="next_preset_button">
               <TOOLTIP text="Next Preset"/>
          </INVISIBLE_ACTION_BUTTON>
       </TPL_TOOLBAR_BUTTON>
      </ROW>
      <CELL flex="1"/>
      <!-- help, about and site -->
			<ROW spacing="0">
				<TPL_TOOLBAR_ITEM image="$THEME_PATH$/toolbar/help.png" action="OpenManual" text="Manual" opacity="$icon_opacity$"/>
				<TPL_TOOLBAR_ITEM image="$THEME_PATH$/toolbar/about.png" action="DisplayAbout" text="About" opacity="$icon_opacity$"/>
			</ROW>
		</IMAGE>
	</ROW>
	</TEMPLATE>
	
  <!-- Preset Name: build from the actual name and additional "*" if modified -->
	<STRING id="theme.toolbar.preset_display_name"/>
	<STRING id="theme.toolbar.preset_modified_string"/>
	<PARAM id="theme.toolbar.preset_status" type="enumeration" enum_values=";*"/>
	<PARAM_LINK from="current_preset.modified" to="theme.toolbar.preset_status"/>
	<PARAM_TO_STRING_LINK from="theme.toolbar.preset_status" to="theme.toolbar.preset_modified_string"/>
	<STRING_MULTI_LINK from="current_preset.name;theme.toolbar.preset_modified_string" to="theme.toolbar.preset_display_name" separator=" "/>
</DUMMY>