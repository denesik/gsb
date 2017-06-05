import json
from itertools import izip
from PIL import Image, ImageEnhance, ImageChops

gearComponent =         ["small_gear", "gear", "big_gear"]
engineComponent = "electric_engine"

machineTiersMaterials = ["copper", "iron",  "steel",    "uranium",   "titanium", "tungsten"]
machineTiers =          ["denis",  "redis", "redis II", "redis III", "redis IV", "redis V"]
machineTiersSimpl =     ["t0",     "t1",    "t2",       "t3",        "t4",       "t5"]
machineTiersVol =       [32,       64,      128,        256,         512,        1024]

machineTypes =      ["electric furnance", "macerator",        "plate bending machine", "magnetic separator", "chemical reactor"]
machineTypesSimpl = ["furnance",          "macerator",        "bender",                "mag_separator",      "chem_reactor"]
machineBlockTypes = ["BlockAutoCrafter",  "BlockAutoCrafter", "BlockAutoCrafter",      "BlockAutoCrafter",   "BlockAutoCrafter"]

generatorTypes =      ["combustion engine", "steam turbine",    "gas turbine",      "automated furnance"]
generatorBlockTypes = ["BlockAutoCrafter",  "BlockAutoCrafter", "BlockAutoCrafter", "BlockAutoCrafter"]

generatorTiers =      ["primitive", "basic", "advanced", "ultimate"]
generatorTiersSimpl = ["0",         "1",     "2",        "3"]

generatorTypesSimpl = ["fuel_engine", "steam_turbine", "gas_turbine", "burner"]

machineTierSides =      [];
machineTierTopBottoms = [];

metals = ["copper", "cobolt", "iron", "steel", "gold", "lead", "magnesium", "electrum", "platinum", "silver", "solder", "uranium", "tungsten"]
anvilComponent = "anvil"
ballComponent = "ball"
bigSpringComponent = "big_spring"
gearComponent = "gear"
hammerHeadComponent = "hammer_head"
hoeHeadComponent = "hoe_head"
machineTop = "machine_top_bottom_template"
machineSide = "machine_side_template"
metallComponents = [anvilComponent, ballComponent, bigSpringComponent, bigSpringComponent, gearComponent, hammerHeadComponent, hoeHeadComponent]

beginId = 200;
id = beginId;

for metal, tier in izip(machineTiersMaterials, machineTiersSimpl):
    name = machineSide + "_" + tier
    template = Image.open("template_parts/" + machineSide + ".png")
    material = Image.open("materials/material_" + metal + ".png")
    output = ImageChops.multiply(template, material)
    output.save("generated/" + name + ".png")
    machineTierSides.append(name)

for metal, tier in izip(machineTiersMaterials, machineTiersSimpl):
    name = machineTop + "_" + tier
    template = Image.open("template_parts/" + machineTop + ".png")
    material = Image.open("materials/material_" + metal + ".png")
    output = ImageChops.multiply(template, material)
    output.save("generated/" + name + ".png")
    machineTierTopBottoms.append(name)

generatorTierSides =      [machineTierSides[0],      machineTierSides[2],      machineTierSides[4],      machineTierSides[5]];
generatorTierTopBottoms = [machineTierTopBottoms[0], machineTierTopBottoms[2], machineTierTopBottoms[4], machineTierTopBottoms[5]];
        
for component in metallComponents:
    for metal in metals:
        name = component + "_" + metal
        template = Image.open("template_parts/" + component + ".png")
        material = Image.open("materials/material_" + metal + ".png")
        output = ImageChops.multiply(template, material)
        output.save("items/generated/" + name + ".png")
    print component + " texgen done"

data = []
for metal in metals:
    for component in metallComponents:
        name = component + "_" + metal
        full_name = metal + " " + component
        item = {
            "section": "item",
            "id": id,
            "name": name,
            "full_name": full_name,
            "tex": "data/items/generated/"+name+".png"
        }
        data.append(item)
        id += 1


for generator, blockType, generatorSimpl in izip(generatorTypes, generatorBlockTypes, generatorTypesSimpl):
    for tier, tierSimpl, sideTex, tbTex in izip(generatorTiers, generatorTiersSimpl, generatorTierSides, generatorTierTopBottoms):
        name = tier + " " + generator
        nameSimpl = generatorSimpl + "_t" + tierSimpl
        item = { 
           "section": "block",
            "name": nameSimpl,
            "full_name" : name,
            "id": id,
            "tesselator": {
              "type": "TesselatorSolidBlock",
              "tex": ["data/"+nameSimpl+".png",
                "data/"+sideTex+".png",
                "data/"+sideTex+".png",
                "data/"+sideTex+".png",
                "data/"+tbTex+".png",
                "data/"+tbTex+".png"]
            },
            "type":blockType,
            "agents":[{
                "type":"AccessorItem",
	        	"name":"0",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"1",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"2",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"3",
                "size":4
              }],
            "Crafter1":
            {
              "Recipe":"RecipeHand",
              "fast":False,
              "Input":"1",
              "Output":"0"
            },
            "Crafter2":
            {
              "Recipe":"RecipeBurn",
              "fast":True,
              "Input":"2",
              "Output":"2"
            },
            "generate_item":{
              "tex":"data/items/furnance_front.png",
              "name" : name
            },
            "drop":[[name, 1]]
        }
        data.append(item)
        id += 1

for machine, blockType, machineSimpl in izip(machineTypes, machineBlockTypes, machineTypesSimpl):
    for tier, tierSimpl, sideTex, tbTex in izip(machineTiers, machineTiersSimpl, machineTierSides, machineTierTopBottoms):
        name = tier + " " + machine
        nameSimpl = machineSimpl + "_t" + tierSimpl
        item = { 
           "section": "block",
            "name": nameSimpl,
            "full_name" : name,
            "id": id,
            "tesselator": {
              "type": "TesselatorSolidBlock",
              "tex": ["data/"+nameSimpl+".png",
                "data/"+sideTex+".png",
                "data/"+sideTex+".png",
                "data/"+sideTex+".png",
                "data/"+tbTex+".png",
                "data/"+tbTex+".png"]
            },
            "type":blockType,
            "agents":[{
                "type":"AccessorItem",
	        	"name":"0",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"1",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"2",
                "size":4
              },
	            {
                "type":"AccessorItem",
	        	"name":"3",
                "size":4
              }],
            "Crafter1":
            {
              "Recipe":"RecipeHand",
              "fast":False,
              "Input":"1",
              "Output":"0"
            },
            "Crafter2":
            {
              "Recipe":"RecipeBurn",
              "fast":True,
              "Input":"2",
              "Output":"2"
            },
            "generate_item":{
              "tex":"data/items/furnance_front.png",
              "name" : name
            },
            "drop":[[name, 1]]
        }
        data.append(item)
        id += 1

with open('json\generated\AutogenMachineryMod.json', 'w') as outfile:
    json.dump(data, outfile, indent=4, sort_keys=True)

print str(id - beginId) + " objects is done"