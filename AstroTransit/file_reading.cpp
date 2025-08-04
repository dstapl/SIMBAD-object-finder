#include <rapidxml_ext.h> // Template definitions
#include <rapidxml_print.hpp>

#include "file_reading.h"

#include <iostream>
#include <filesystem>

const size_t NUM_FIELD_HEADERS = 246;
// Hopefully both lists contain the same keys since processed in vim
const char* FIELD_HEADERS[NUM_FIELD_HEADERS] = {
    // IDs
    "TYPED_ID",
    "MATCHING_ID",
    "ANG_DIST",
    "MAIN_ID",
    "OTYPE_S",

    // Position
    "RA_d",
    "DEC_d",
    "COO_ERR_MAJA_d",
    "COO_ERR_MINA_d",
    "COO_ERR_ANGLE_d",
    "PMRA",
    "PMDEC",
    // Error in position
    "PM_ERR_MAJA",
    "PM_ERR_MINA",
    "PM_ERR_ANGLE",

    // Something...
    "PLX_VALUE",
    "RV_VALUE",
    "Z_VALUE",
    "VLSR",
    // Extended object dimensions
    "GALDIM_MAJAXIS",
    "GALDIM_MINAXIS",
    "GALDIM_ANGLE",
    "SP_TYPE",
    "MORPH_TYPE",
    "NB_REF",

    // Filters
    // Johnson
    "FILTER_NAME_U",
    "FLUX_U",
    "FLUX_ERROR_U",
    "FLUX_SYSTEM_U",
    "FLUX_BIBCODE_U",
    "FLUX_VAR_U",
    "FLUX_MULT_U",
    "FLUX_QUAL_U",
    "FLUX_UNIT_U",
    "FILTER_NAME_B",
    "FLUX_B",
    "FLUX_ERROR_B",
    "FLUX_SYSTEM_B",
    "FLUX_BIBCODE_B",
    "FLUX_VAR_B",
    "FLUX_MULT_B",
    "FLUX_QUAL_B",
    "FLUX_UNIT_B",
    "FILTER_NAME_V",
    "FLUX_V",
    "FLUX_ERROR_V",
    "FLUX_SYSTEM_V",
    "FLUX_BIBCODE_V",
    "FLUX_VAR_V",
    "FLUX_MULT_V",
    "FLUX_QUAL_V",
    "FLUX_UNIT_V",
    "FILTER_NAME_G",
    "FLUX_G",
    "FLUX_ERROR_G",
    "FLUX_SYSTEM_G",
    "FLUX_BIBCODE_G",
    "FLUX_VAR_G",
    "FLUX_MULT_G",
    "FLUX_QUAL_G",
    "FLUX_UNIT_G",
    "FILTER_NAME_R",
    "FLUX_R",
    "FLUX_ERROR_R",
    "FLUX_SYSTEM_R",
    "FLUX_BIBCODE_R",
    "FLUX_VAR_R",
    "FLUX_MULT_R",
    "FLUX_QUAL_R",
    "FLUX_UNIT_R",
    "FILTER_NAME_I",
    "FLUX_I",
    "FLUX_ERROR_I",
    "FLUX_SYSTEM_I",
    "FLUX_BIBCODE_I",
    "FLUX_VAR_I",
    "FLUX_MULT_I",
    "FLUX_QUAL_I",
    "FLUX_UNIT_I",
    "FILTER_NAME_J",
    "FLUX_J",
    "FLUX_ERROR_J",
    "FLUX_SYSTEM_J",
    "FLUX_BIBCODE_J",
    "FLUX_VAR_J",
    "FLUX_MULT_J",
    "FLUX_QUAL_J",
    "FLUX_UNIT_J",
    "FILTER_NAME_H",
    "FLUX_H",
    "FLUX_ERROR_H",
    "FLUX_SYSTEM_H",
    "FLUX_BIBCODE_H",
    "FLUX_VAR_H",
    "FLUX_MULT_H",
    "FLUX_QUAL_H",
    "FLUX_UNIT_H",
    "FILTER_NAME_K",
    "FLUX_K",
    "FLUX_ERROR_K",
    "FLUX_SYSTEM_K",
    "FLUX_BIBCODE_K",
    "FLUX_VAR_K",
    "FLUX_MULT_K",
    "FLUX_QUAL_K",
    "FLUX_UNIT_K",
    "FILTER_NAME_u",

    // Sloan filters
    "FLUX_u",
    "FLUX_ERROR_u",
    "FLUX_SYSTEM_u",
    "FLUX_BIBCODE_u",
    "FLUX_VAR_u",
    "FLUX_MULT_u",
    "FLUX_QUAL_u",
    "FLUX_UNIT_u",
    "FILTER_NAME_g",
    "FLUX_g",
    "FLUX_ERROR_g",
    "FLUX_SYSTEM_g",
    "FLUX_BIBCODE_g",
    "FLUX_VAR_g",
    "FLUX_MULT_g",
    "FLUX_QUAL_g",
    "FLUX_UNIT_g",
    "FILTER_NAME_r",
    "FLUX_r",
    "FLUX_ERROR_r",
    "FLUX_SYSTEM_r",
    "FLUX_BIBCODE_r",
    "FLUX_VAR_r",
    "FLUX_MULT_r",
    "FLUX_QUAL_r",
    "FLUX_UNIT_r",
    "FILTER_NAME_i",
    "FLUX_i",
    "FLUX_ERROR_i",
    "FLUX_SYSTEM_i",
    "FLUX_BIBCODE_i",
    "FLUX_VAR_i",
    "FLUX_MULT_i",
    "FLUX_QUAL_i",
    "FLUX_UNIT_i",
    "FILTER_NAME_z",
    "FLUX_z",
    "FLUX_ERROR_z",
    "FLUX_SYSTEM_z",
    "FLUX_BIBCODE_z",
    "FLUX_VAR_z",
    "FLUX_MULT_z",
    "FLUX_QUAL_z",
    "FLUX_UNIT_z",

    // Object diameter
    "Diameter_diameter",
    "Diameter_Q",
    "Diameter_unit",
    "Diameter_error",
    "Diameter_filter",
    "Diameter_method",
    "Diameter_bibcode",
    "Distance_distance",
    "Distance_Q",
    "Distance_unit",
    "Distance_merr",
    "Distance_perr",
    "Distance_method",
    "Distance_bibcode",

    // Metallicity derived properties
    "Fe_H_Teff",
    "Fe_H_log_g",
    "Fe_H_Fe_H",
    "Fe_H_flag",
    "Fe_H_CompStar",
    "Fe_H_CatNo",
    "Fe_H_bibcode",

    // Observation record properties
    "Herschel_ObsId",
    "Herschel_alpha",
    "Herschel_delta",
    "PLX_plx",
    "PLX_me",
    "PLX_R",
    "PLX_bibcode",


    "PM_pmra",
    "PM_me_pmra",
    "PM_pmde",
    "PM_me_pmde",
    "PM_system",
    "PM_bibcode",
    "ROT_upVsini",
    "ROT_Vsini",
    "ROT_err",
    "ROT_mes",
    "ROT_qual",
    "ROT_bibcode",


    "V__vartyp",
    "V__Lomax",
    "V__max",
    "V__R_max",
    "V__band",
    "V__Upmin",
    "V__min",
    "V__R_min",
    "V__UpPeriod",
    "V__period",
    "V__R_period",
    "V__epoch",
    "V__R_epoch",
    "V__D_rt",
    "V__R_D_rt",
    "V__bibcode",


    "velocities_type",
    "velocities_Value",
    "velocities_R",
    "velocities_me",
    "velocities_Acc",
    "velocities_Nmes",
    "velocities_nat",
    "velocities_Q",
    "velocities_dom",
    "velocities_res",
    "velocities_d",
    "velocities_Date",
    "velocities_Rem",
    "velocities_Origin",
    "velocities_bibcode",


    "ISO_TDT",
    "ISO_alpha",
    "ISO_delta",
    "IUE_Homogenized_Name",
    "IUE_ComplID",
    "IUE_PROG",
    "IUE_CL",
    "IUE_D",
    "IUE_CAM",
    "IUE_IMAGE",
    "IUE_A",
    "IUE_FES",
    "IUE_MD",
    "IUE_ObsDate",
    "IUE_Time",
    "IUE_ExpTim",
    "IUE_m",
    "IUE_CEB",
    "IUE_S",
    "IUE_Comments",
    "IUE_F",
    "IUE_bibcode",
    "OID4",
    "XMM_Obsno",
};


// Find first child node with a given name
static rapidxml::xml_node<>* find_child(rapidxml::xml_node<>* parent, const char* name) {
	for (rapidxml::xml_node<>* node = parent ? parent->first_node() : nullptr; node; node = node->next_sibling()) {
		if (std::string(node->name()) == name)
			return node;
	}
	return nullptr;
}

Row::Row(rapidxml::xml_node<>* tablerow) {
	size_t fieldIndex = 0;

	for (rapidxml::xml_node<>* td = tablerow ? tablerow->first_node("TD") : nullptr;
		td && fieldIndex < NUM_FIELD_HEADERS;
		td = td->next_sibling("TD"), ++fieldIndex)
	{
		std::string value = td->value();
		// If TD is empty keep map key-value unset
		if (value.empty()) continue;

		fields[FIELD_HEADERS[fieldIndex]] = value;
	}
};

bool VOTable::load(const std::filesystem::path& filepath) {
    if (!std::filesystem::exists(filepath) ||
        !std::filesystem::is_regular_file(filepath))
        return false;

    // Create filestream
    uintmax_t size = std::filesystem::file_size(filepath);
    std::ifstream inFile(filepath, std::ios::binary);
    if (!inFile) return false;

    auto xmlFile = rapidxml::file<>(inFile);

    using namespace rapidxml;

    // Pre-allocates 64kb on heap by initialising alone
    auto doc = std::make_unique< xml_document<> >();
    doc->parse<0>(xmlFile.data()); // Parses *entire* DOM


    // Find the TABLEDATA node
    xml_node<>* td = find_child(
        find_child(
            find_child(
                find_child(
                    doc->first_node("VOTABLE"),
                    "RESOURCE"
                ),
                "TABLE"
            ),
            "DATA"
        ),
        "TABLEDATA"
    );
    if (!td) return 1;


    // Populate tableData with parsed rows
    tableData.clear();
    // TODO: Estimate row count and reserve? Probably at least 100 rows.
    for (xml_node<>* tr = td->first_node("TR"); tr; tr = tr->next_sibling("TR")) {
        // emplace_back? or push_back?
        tableData.emplace_back(tr);
    }

    return 0;
};

void VOTable::printTable() {
    for (const auto& row : tableData) {
        for (const auto& [key, value] : row.fields) {
            std::cout << key << ": " << value << "  ";
        }
        std::cout << std::endl;
    }
}
