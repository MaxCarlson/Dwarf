
grass_lifecycle = { 10, 28, 90, 90, 3 };
fast_lifecycle  = { 10, 28, 60, 60, 3 }; ----------------------------- Add in a grown by  dwarves glyph set to sepperate out tilled soil glyphs
slow_lifecycle  = { 28, 40, 90, 90, 0 };

function build_plants(name, cycles, glyphs, harvest, tags) return { name = name, cycles = cycles, glyphs = glyphs, harvest = harvest, tags = tags; } end

function harvest_normal(h) return {"none", "none", h, h } end
function harvest_bush(h) return {"none", "none", h, "none" } end

function veg_a(a, ac, b, bc, c, cc, d, dc) return 
{ 
	a = { glyph = glyphs[a], col = ac }, 
	b = { glyph = glyphs[b], col = bc }, 
	c = { glyph = glyphs[c], col = cc },
	d = { glyph = glyphs[d], col = dc }
} end


grass_glyphs = veg_a('grass', "brown", 'grass', "green",  'grass', "brown", 'grass',  "brown"  );

function flower_glyphs(color) return veg_a('grass', "brown", 'sprout', "green", 'flower', color, 'sprout', "yellow" ); end

function berry_bush_glyphs(color) return veg_a('sprout', "brown", 'bush', "green", 'berry_bush', color, 'bush', "yellow" ); end

function vegetable_glyphs(color) return veg_a('tilled', "brown", 'tilled_stakes', "brown", 'tilled_growth', "brown", 'tilled_growth', "brown"); end

vegetation = {

	-- Grass and flowers
	grass = build_plants("Grass", grass_lifecycle, grass_glyphs, harvest_normal('none'), {'spread'}),
	tulip = build_plants("Tulip", grass_lifecycle, flower_glyphs("purple"), harvest_normal('none'), {'spread'}),


	-- Bush like plants
	black_berry = build_plants("Black Berrys", slow_lifecycle, berry_bush_glyphs("purple"), harvest_bush('black_berry'), {'annual'}),


	-- Vegetables
	potato = build_plants("Potato", fast_lifecycle, vegetable_glyphs('none'), harvest_normal('potato'), {''}),
};