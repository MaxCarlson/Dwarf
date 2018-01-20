
grass_lifecycle = { 10, 28, 90, 90, 3 };
fast_lifecycle  = { 10, 28, 60, 60, 3 };
slow_lifecycle  = { 28, 40, 90, 90, 0 };

function build_plants(name, cycles, glyphs, harvest, tags) return { name = name, cycles = cycles, glyphs = glyphs, harvest = harvest, tags = tags; } end

function harvest_normal(h) return {"none", "none", h, h } end

function veg_a(a, ac, b, bc, c, cc, d, dc) return { a = { glyphs[a], col = ac }, b = { glyphs[b], col = bc }, c = { glyphs[c], col = cc }, d = { glyphs[d], col = dc }} end


grass_glyphs = vega_a('grass', "brown", 'grass', "green",  'grass', "brown", 'grass',  "brown"  );

--function flower_glyphs(color) return veg_a('grass', "brown", 'sprout', "green", 'flower', color, 'sprout', "yellow" ); end

vegetation = {

	grass = build_plants("Grass", grass_lifecycle, grass_glyphs, harvest_normal('none'), {'spread'}),
	--tulip = build_plants("Tulip", grass_lifecycle, flower_glyphs("purple"), harvest_normal('none'), {'spread'}),
};