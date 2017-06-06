import json
from itertools import izip
from PIL import Image, ImageEnhance, ImageChops

gearComponent = ["small_gear", "gear", "big_gear"]
engineComponent = "electric_engine"

machineTiersMaterials = ["copper", "iron",  "steel",    "uranium",   "titanium", "tungsten"]
machineTiers = ["denis",  "redis", "redis II", "redis III", "redis IV", "redis V"]
machineTiersSimpl = ["t0",     "t1",    "t2",       "t3",        "t4",       "t5"]
machineTiersVol = [32,       64,      128,        256,         512,        1024]
machineAccCount = [4,        4,       4,          4,           4,          4]
machineAccSize = [4,        4,       4,          4,           4,          4]

machineTypes = ["electric furnance", "macerator",        "plate bending machine", "magnetic separator", "chemical reactor"]
machineTypesSimpl = ["furnance",          "macerator",        "bender",                "mag_separator",      "chem_reactor"]
machineBlockTypes = ["BlockAutoCrafter",  "BlockAutoCrafter", "BlockAutoCrafter",      "BlockAutoCrafter",   "BlockAutoCrafter"]

generatorTypes = ["combustion engine", "steam turbine",    "gas turbine",      "automated furnance"]
generatorBlockTypes = ["BlockResourceGenerator",  "BlockResourceGenerator", "BlockResourceGenerator", "BlockResourceGenerator"]

generatorTiers = ["primitive", "basic", "advanced", "ultimate"]
generatorTiersSimpl = ["0",         "1",     "2",        "3"]
generatorAccSize = [1,           4,       9,          16]
generatorTiersMaterials = [machineTiersMaterials[1], machineTiersMaterials[2], machineTiersMaterials[4], machineTiersMaterials[5]]

generatorTypesSimpl = ["fuel_engine",    "steam_turbine", "gas_turbine",  "burner"]
generatorResType = ["rotation",       "rotation",      "rotation",     "heat"]
generatorAccType = ["AccessorItem", "AccessorItem",  "AccessorItem",  "AccessorItem"]

metals = ["copper", "cobolt", "iron", "steel", "gold", "lead", "magnesium", "electrum", "platinum", "silver", "solder", "uranium", "tungsten"]
anvilComponent = "anvil"
ballComponent = "ball"
bigSpringComponent = "big_spring"
gearComponent = "gear"
hammerHeadComponent = "hammer_head"
hoeHeadComponent = "hoe_head"
oreComponent = "ore"
ingotComponent = "ingot"
machineTop = "machine_top_bottom_template"
machineSide = "machine_side_template"
metallComponents = [anvilComponent, ballComponent, bigSpringComponent, bigSpringComponent, gearComponent, hammerHeadComponent, hoeHeadComponent, oreComponent, ingotComponent]

beginId = 200
id = beginId

machineTierSides = []
for metal, tier in izip(machineTiersMaterials, machineTiersSimpl):
    name = machineSide + "_" + tier
    template = Image.open("template_parts/" + machineSide + ".png")
    material = Image.open("materials/material_" + metal + ".png")
    output = ImageChops.multiply(template, material)
    output.save("generated/" + name + ".png")
    machineTierSides.append(name)

machineTierTopBottoms = []
for metal, tier in izip(machineTiersMaterials, machineTiersSimpl):
    name = machineTop + "_" + tier
    template = Image.open("template_parts/" + machineTop + ".png")
    material = Image.open("materials/material_" + metal + ".png")
    output = ImageChops.multiply(template, material)
    output.save("generated/" + name + ".png")
    machineTierTopBottoms.append(name)

generatorTierSides = [machineTierSides[0],      machineTierSides[2],      machineTierSides[4],      machineTierSides[5]]
generatorTierTopBottoms = [machineTierTopBottoms[0], machineTierTopBottoms[2], machineTierTopBottoms[4], machineTierTopBottoms[5]]
        
for component in metallComponents:
    for metal in metals:
        name = component + "_" + metal
        template = Image.open("template_parts/" + component + ".png")
        material = Image.open("materials/material_" + metal + ".png")
        output = ImageChops.multiply(template, material)
        output.save("items/generated/" + name + ".png")
    print component + " texgen done"

data = []

data.append({
            "section": "item",
            "id": id,
            "name": "heat",
            "full_name": "heat"
            })
id += 1

data.append({
            "section": "item",
            "id": id,
            "name": "rotation",
            "full_name": "rotation"
            })
id += 1

data.append({
            "section": "item",
            "id": id,
            "name": "electricity",
            "full_name": "electricity"
            })
id += 1

data.append(
  {
    "section": "recipe",
    "type": "RecipeResource",
    "id": 5,
    "time": 35.0,
    "input": [ [ "coke", 1 ] ],
    "output": [ [ "heat", 1] ],
    "resource": "heat"
  })
data.append({
    "section": "recipe",
    "type": "RecipeResource",
    "id": 6,
    "time": 35.0,
    "input": [ [ "coal", 1 ] ],
    "output": [ [ "heat", 1] ],
    "resource": "heat"
  })

for metal in metals:
    for component in metallComponents:
        name = component + "_" + metal
        full_name = metal + " " + component
        item = {
            "section": "item",
            "id": id,
            "name": name,
            "full_name": full_name,
            "tex": "data/items/generated/" + name + ".png"
        }
        data.append(item)
        id += 1


for g in range(0, len(generatorTypes)):
    generator = generatorTypes[g]
    blockType = generatorBlockTypes[g]
    generatorSimpl = generatorTypesSimpl[g]
    accSize = generatorAccSize[g]
    resType = generatorResType[g]
    accType = generatorAccType[g]
    for tier, tierSimpl, sideTex, tbTex, generatorMaterial in izip(generatorTiers, generatorTiersSimpl, generatorTierSides, generatorTierTopBottoms, generatorTiersMaterials):
        name = tier + " " + generator
        nameSimpl = generatorSimpl + "_t" + tierSimpl
        item = {
            "section": "item",
            "id": id,
            "name": nameSimpl + "_block",
            "full_name": name,
            "tex":"data/items/generated/"+nameSimpl+".png",
            "block": id + 1
        }
        data.append(item)
        id += 1

        block = { 
           "section": "block",
            "name": nameSimpl,
            "full_name" : name,
            "id": id,
            "tesselator": {
              "type": "TesselatorSolidBlock",
              "tex": ["data/" + nameSimpl + ".png",
                "data/generated/" + sideTex + ".png",
                "data/generated/" + sideTex + ".png",
                "data/generated/" + sideTex + ".png",
                "data/generated/" + tbTex + ".png",
                "data/generated/" + tbTex + ".png"]
            },
            "type":blockType,
            "accessors":[{
                "type":accType,
	        	"name":"0",
                "size":accSize
              },{
                "type":accType,
	        	"name":"1",
                "size":1
              },{
                "type":"AccessorResource",
	        	"name":"2",
                "size":accSize,
                "resource":resType
            }],
            "Generator1":
            {
              "Recipe":"RecipeResource",
              "fast":False,
              "Input":"1",
              "Output":"0",
              "resource":resType
            },
            "resource":resType,
            "drop":[[name, 1]]
        }
        data.append(block)
        id += 1

        template = Image.open("template_parts/" + generatorSimpl + ".png")
        material = Image.open("materials/material_" + generatorMaterial + ".png")
        output = ImageChops.multiply(template, material)
        output.save("items/generated/" + nameSimpl + ".png")
        output.save("generated/" + nameSimpl + ".png")

for machine, blockType, machineSimpl, accCount, accSize in izip(machineTypes, machineBlockTypes, machineTypesSimpl, machineAccCount, machineAccSize):
    for tier, tierSimpl, sideTex, tbTex, tierMaterial in izip(machineTiers, machineTiersSimpl, machineTierSides, machineTierTopBottoms, machineTiersMaterials):
        name = tier + " " + machine
        nameSimpl = machineSimpl + "_" + tierSimpl
        item = { 
           "section": "block",
            "name": nameSimpl,
            "full_name" : name,
            "id": id,
            "tesselator": {
              "type": "TesselatorSolidBlock",
              "tex": ["data/generated/" + nameSimpl + ".png",
                "data/generated/" + sideTex + ".png",
                "data/generated/" + sideTex + ".png",
                "data/generated/" + sideTex + ".png",
                "data/generated/" + tbTex + ".png",
                "data/generated/" + tbTex + ".png"]
            },
            "type":blockType,
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
              "Input":"1",
              "Output":"0",
              "resource":resType
            },
            "generate_item":{
              "tex":"data/items/generated/"+nameSimpl+".png",
              "name" : name
            },
            "drop":[[name, 1]],
        }
        item["accessors"] = [{"type":"AccessorItem", "name":"0", "size":accSize}]
        for i in range(1, accCount):
            item["accessors"].append({"type":"AccessorItem", "name":str(i), "size":accSize})
        data.append(item)
        id += 1

        template = Image.open("template_parts/" + machineSimpl + ".png")
        material = Image.open("materials/material_" + tierMaterial + ".png")
        output = ImageChops.multiply(template, material)
        output.save("items/generated/" + nameSimpl + ".png")
        output.save("generated/" + nameSimpl + ".png")

with open('json\generated\AutogenMachineryMod.json', 'w') as outfile:
    json.dump(data, outfile, indent=4, sort_keys=True)

print str(id - beginId) + " objects is done"