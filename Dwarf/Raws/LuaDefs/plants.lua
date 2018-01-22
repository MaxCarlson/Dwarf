
grass_lifecycle = { 10, 28, 90, 90, 3 };
fast_lifecycle  = { 10, 28, 60, 60, 3 }; ----------------------------- Add in a grown by  dwarves glyph set to sepperate out tilled soil glyphs as well as time and difficulty?
slow_lifecycle  = { 28, 40, 90, 90, 0 };

function build_plants(name, cycles, glyphs, harvest, tags, difficulty) return { name = name, cycles = cycles, glyphs = glyphs, harvest = harvest, tags = tags, difficulty = difficulty; } end

function harvest_normal(h) return {"none", "none", h, h } end
function harvest_bush(h) return {"none", "none", h, "none" } end

function veg_a(a, ac, b, bc, c, cc, d, dc) return 
{ 
	a = { glyph = glyphs[a], col = ac }, 
	b = { glyph = glyphs[b], col = bc }, 
	c = { glyph = glyphs[c], col = cc },
	d = { glyph = glyphs[d], col = dc }
} end

difficulty_simple = { diff = 6,  ptime = 2, htime = 3, lvl = 0 };
difficulty_easy   = { diff = 8,  ptime = 4, htime = 5, lvl = 3 };
difficulty_normal = { diff = 11, ptime = 5, htime = 6, lvl = 5 };
difficulty_hard   = { diff = 14, ptime = 6, htime = 8  lvl = 8 };

grass_glyphs = veg_a('grass', "light grass_green", 'grass', "dark grass_green",  'grass', "grass_green", 'grass', "dead_grass" );

function flower_glyphs(color) return veg_a('grass', "brown", 'sprout', "green", 'flower', color, 'sprout', "yellow" ); end

function berry_bush_glyphs(color) return veg_a('sprout', "brown", 'bush', "green", 'berry_bush', color, 'bush', "yellow" ); end

function root_glyphs(color) return veg_a('sprout', "brown", 'flower', "brown", 'root_veg', color, 'root_veg', "brown"); end

function mushroom_glyphs(color) return veg_a('grass', "brown", 'one_mush', "brown", 'mulit_mush', color, 'mulit_mush', color); end

vegetation = {

	-- Grass and flowers
	grass = build_plants("Grass", grass_lifecycle, grass_glyphs, harvest_normal('blackberry'), {'spread'}, difficulty_simple),
	tulip = build_plants("Tulip", grass_lifecycle, flower_glyphs("purple"), harvest_normal('none'), {'spread'}, difficulty_simple),


	-- Bush like plants
	blackberry = build_plants("Black Berrys", slow_lifecycle, berry_bush_glyphs("purple"), harvest_bush('blackberry'), {'annual'}, difficulty_normal),


	-- Vegetables
	potato = build_plants("Potato", fast_lifecycle, root_glyphs('green'), harvest_normal('potato'), {''}, difficulty_simple),

	-- Mushrooms
	devil_strand = build_plants("Devil Strand", slow_lifecycle, mushroom_glyphs("dark brown"), harvest_normal('devil_strand'), {''}, difficulty_hard),
};