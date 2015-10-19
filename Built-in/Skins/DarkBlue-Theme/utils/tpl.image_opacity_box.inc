<?xml version="1.0" encoding="utf-8" ?>
<DUMMY>
  <TEMPLATE id="_TPL_IMAGE_OPACITY_BOX" layout_type="row" spacing="5" h_margin="10" v_margin="5" opacity="60%" width="-1" height="-1" image="$THEME_PATH$/pads/pad-bluelight.png" v_align="center" h_align="center" flex="0" font_size="+0" internal_h_align="center" internal_v_align="center" mouse_sensitive="false" reflow="false">
      <!-- group box with opacity -->
      <LAYER_STACK width="$width$" height="$height$" v_align="$v_align$" h_align="$h_align$" flex="$flex$" id="$id$" font_size="$font_size$">
          <IMAGE_GROUP_BOX image="$image$" stretch="true" opacity="$opacity$" width="100%" height="100%" mouse_sensitive="$mouse_sensitive$"/>
          <CELL layout_type="$layout_type$" spacing="$spacing$" h_margin="$h_margin$" v_margin="$v_margin$" width="$width$" height="$height$" internal_v_align="$internal_v_align$" internal_h_align="$internal_h_align$" flex="$flex$" reflow="$reflow$">
            <TEMPLATE_INNER_CONTENT/>
          </CELL>
      </LAYER_STACK>
  </TEMPLATE>
  
  <DEFINE>
    <TPL_IMAGE_OPACITY_BOX base_type="_TPL_IMAGE_OPACITY_BOX" id=""/>
  </DEFINE>
</DUMMY>