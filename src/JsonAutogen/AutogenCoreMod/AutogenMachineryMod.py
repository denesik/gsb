import json
from itertools import izip

gearComponent =         ["small_gear", "gear", "big_gear"]
engineComponent = "electric_engine"

machineTiersMaterials = ["bronze",       "iron",  "steel",    "ss",        "titan",    "denisian"]
machineTiers =          ["denis",        "redis", "redis II", "redis III", "redis IV", "redis V"]
machineTiersSimpl =     ["0",            "1",     "2",        "3",         "4",        "5"]
machineTiersVol =       ["32",           "64",    "128",      "256",       "512",      "1024"]

machineTypes =      ["electric furnance", "macerator",        "plate bending machine", "magnetic separator", "chemical reactor"]
machineTypesSimpl = ["furnance",          "macerator",        "bender",                "mag_separator",      "chemReactor"]
machineBlockTypes = ["BlockAutoCrafter",  "BlockAutoCrafter", "BlockAutoCrafter",      "BlockAutoCrafter",   "BlockAutoCrafter"]

generatorTypes =      ["combustion engine", "steam turbine",    "gas turbine",      "automated furnance"]
generatorBlockTypes = ["BlockAutoCrafter",  "BlockAutoCrafter", "BlockAutoCrafter", "BlockAutoCrafter"]

generatorTiers =      ["primitive", "basic", "advanced", "ultimate"]
generatorTiersSimpl = ["0",         "1",     "2",        "3"]

generatorTypesSimpl = ["fuel_engine", "steam_turbine", "gas_turbine", "burner"]

machineTierSides =      ["furnance_side1", "furnance_side2", "furnance_side3", "furnance_side4", "furnance_side5", "furnance_side6"];
machineTierTopBottoms = ["furnance_top_bottom1", "furnance_top_bottom2", "furnance_top_bottom3", "furnance_top_bottom4", "furnance_top_bottom5", "furnance_top_bottom6"];

generatorTierSides =      [machineTierSides[0],      machineTierSides[2],      machineTierSides[4],      machineTierSides[5]];
generatorTierTopBottoms = [machineTierTopBottoms[0], machineTierTopBottoms[2], machineTierTopBottoms[4], machineTierTopBottoms[5]];

beginId = 200;
id = beginId;

data = []
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

with open('AutogenMachineryMod.json', 'w') as outfile:
    json.dump(data, outfile, indent=4, sort_keys=True)

print str(id - beginId) + " objects is done"