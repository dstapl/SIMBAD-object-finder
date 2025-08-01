#pragma once
// TODO: Are these all really necessary...?
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <optional>
#include <variant>

#include <rapidxml_utils.hpp>

enum FieldType {
    Char, // String / char*
    Float,
    Double,
    Short,
    Int,
};


extern const char* FIELD_HEADERS[246];

// Copied from XML definitions
static const std::unordered_map<std::string, FieldType> FIELD_TYPE_MAP = {
    {"TYPED_ID", FieldType::Char},
    {"MATCHING_ID", FieldType::Char},
    {"ANG_DIST", FieldType::Float},
    {"MAIN_ID", FieldType::Char},
    {"OTYPE_S", FieldType::Char},
    {"RA_d", FieldType::Double},
    {"DEC_d", FieldType::Double},
    {"COO_ERR_MAJA_d", FieldType::Float},
    {"COO_ERR_MINA_d", FieldType::Float},
    {"COO_ERR_ANGLE_d", FieldType::Short},
    {"PMRA", FieldType::Double},
    {"PMDEC", FieldType::Double},
    {"PM_ERR_MAJA", FieldType::Float},
    {"PM_ERR_MINA", FieldType::Float},
    {"PM_ERR_ANGLE", FieldType::Short},
    {"PLX_VALUE", FieldType::Double},
    {"RV_VALUE", FieldType::Double},
    {"Z_VALUE", FieldType::Double},
    {"VLSR", FieldType::Double},
    {"GALDIM_MAJAXIS", FieldType::Float},
    {"GALDIM_MINAXIS", FieldType::Float},
    {"GALDIM_ANGLE", FieldType::Short},
    {"SP_TYPE", FieldType::Char},
    {"MORPH_TYPE", FieldType::Char},
    {"NB_REF", FieldType::Int},
    {"FILTER_NAME_U", FieldType::Char},
    {"FLUX_U", FieldType::Float},
    {"FLUX_ERROR_U", FieldType::Float},
    {"FLUX_SYSTEM_U", FieldType::Char},
    {"FLUX_BIBCODE_U", FieldType::Char},
    {"FLUX_VAR_U", FieldType::Char},
    {"FLUX_MULT_U", FieldType::Char},
    {"FLUX_QUAL_U", FieldType::Char},
    {"FLUX_UNIT_U", FieldType::Char},
    {"FILTER_NAME_B", FieldType::Char},
    {"FLUX_B", FieldType::Float},
    {"FLUX_ERROR_B", FieldType::Float},
    {"FLUX_SYSTEM_B", FieldType::Char},
    {"FLUX_BIBCODE_B", FieldType::Char},
    {"FLUX_VAR_B", FieldType::Char},
    {"FLUX_MULT_B", FieldType::Char},
    {"FLUX_QUAL_B", FieldType::Char},
    {"FLUX_UNIT_B", FieldType::Char},
    {"FILTER_NAME_V", FieldType::Char},
    {"FLUX_V", FieldType::Float},
    {"FLUX_ERROR_V", FieldType::Float},
    {"FLUX_SYSTEM_V", FieldType::Char},
    {"FLUX_BIBCODE_V", FieldType::Char},
    {"FLUX_VAR_V", FieldType::Char},
    {"FLUX_MULT_V", FieldType::Char},
    {"FLUX_QUAL_V", FieldType::Char},
    {"FLUX_UNIT_V", FieldType::Char},
    {"FILTER_NAME_G", FieldType::Char},
    {"FLUX_G", FieldType::Float},
    {"FLUX_ERROR_G", FieldType::Float},
    {"FLUX_SYSTEM_G", FieldType::Char},
    {"FLUX_BIBCODE_G", FieldType::Char},
    {"FLUX_VAR_G", FieldType::Char},
    {"FLUX_MULT_G", FieldType::Char},
    {"FLUX_QUAL_G", FieldType::Char},
    {"FLUX_UNIT_G", FieldType::Char},
    {"FILTER_NAME_R", FieldType::Char},
    {"FLUX_R", FieldType::Float},
    {"FLUX_ERROR_R", FieldType::Float},
    {"FLUX_SYSTEM_R", FieldType::Char},
    {"FLUX_BIBCODE_R", FieldType::Char},
    {"FLUX_VAR_R", FieldType::Char},
    {"FLUX_MULT_R", FieldType::Char},
    {"FLUX_QUAL_R", FieldType::Char},
    {"FLUX_UNIT_R", FieldType::Char},
    {"FILTER_NAME_I", FieldType::Char},
    {"FLUX_I", FieldType::Float},
    {"FLUX_ERROR_I", FieldType::Float},
    {"FLUX_SYSTEM_I", FieldType::Char},
    {"FLUX_BIBCODE_I", FieldType::Char},
    {"FLUX_VAR_I", FieldType::Char},
    {"FLUX_MULT_I", FieldType::Char},
    {"FLUX_QUAL_I", FieldType::Char},
    {"FLUX_UNIT_I", FieldType::Char},
    {"FILTER_NAME_J", FieldType::Char},
    {"FLUX_J", FieldType::Float},
    {"FLUX_ERROR_J", FieldType::Float},
    {"FLUX_SYSTEM_J", FieldType::Char},
    {"FLUX_BIBCODE_J", FieldType::Char},
    {"FLUX_VAR_J", FieldType::Char},
    {"FLUX_MULT_J", FieldType::Char},
    {"FLUX_QUAL_J", FieldType::Char},
    {"FLUX_UNIT_J", FieldType::Char},
    {"FILTER_NAME_H", FieldType::Char},
    {"FLUX_H", FieldType::Float},
    {"FLUX_ERROR_H", FieldType::Float},
    {"FLUX_SYSTEM_H", FieldType::Char},
    {"FLUX_BIBCODE_H", FieldType::Char},
    {"FLUX_VAR_H", FieldType::Char},
    {"FLUX_MULT_H", FieldType::Char},
    {"FLUX_QUAL_H", FieldType::Char},
    {"FLUX_UNIT_H", FieldType::Char},
    {"FILTER_NAME_K", FieldType::Char},
    {"FLUX_K", FieldType::Float},
    {"FLUX_ERROR_K", FieldType::Float},
    {"FLUX_SYSTEM_K", FieldType::Char},
    {"FLUX_BIBCODE_K", FieldType::Char},
    {"FLUX_VAR_K", FieldType::Char},
    {"FLUX_MULT_K", FieldType::Char},
    {"FLUX_QUAL_K", FieldType::Char},
    {"FLUX_UNIT_K", FieldType::Char},
    {"FILTER_NAME_u", FieldType::Char},
    {"FLUX_u", FieldType::Float},
    {"FLUX_ERROR_u", FieldType::Float},
    {"FLUX_SYSTEM_u", FieldType::Char},
    {"FLUX_BIBCODE_u", FieldType::Char},
    {"FLUX_VAR_u", FieldType::Char},
    {"FLUX_MULT_u", FieldType::Char},
    {"FLUX_QUAL_u", FieldType::Char},
    {"FLUX_UNIT_u", FieldType::Char},
    {"FILTER_NAME_g", FieldType::Char},
    {"FLUX_g", FieldType::Float},
    {"FLUX_ERROR_g", FieldType::Float},
    {"FLUX_SYSTEM_g", FieldType::Char},
    {"FLUX_BIBCODE_g", FieldType::Char},
    {"FLUX_VAR_g", FieldType::Char},
    {"FLUX_MULT_g", FieldType::Char},
    {"FLUX_QUAL_g", FieldType::Char},
    {"FLUX_UNIT_g", FieldType::Char},
    {"FILTER_NAME_r", FieldType::Char},
    {"FLUX_r", FieldType::Float},
    {"FLUX_ERROR_r", FieldType::Float},
    {"FLUX_SYSTEM_r", FieldType::Char},
    {"FLUX_BIBCODE_r", FieldType::Char},
    {"FLUX_VAR_r", FieldType::Char},
    {"FLUX_MULT_r", FieldType::Char},
    {"FLUX_QUAL_r", FieldType::Char},
    {"FLUX_UNIT_r", FieldType::Char},
    {"FILTER_NAME_i", FieldType::Char},
    {"FLUX_i", FieldType::Float},
    {"FLUX_ERROR_i", FieldType::Float},
    {"FLUX_SYSTEM_i", FieldType::Char},
    {"FLUX_BIBCODE_i", FieldType::Char},
    {"FLUX_VAR_i", FieldType::Char},
    {"FLUX_MULT_i", FieldType::Char},
    {"FLUX_QUAL_i", FieldType::Char},
    {"FLUX_UNIT_i", FieldType::Char},
    {"FILTER_NAME_z", FieldType::Char},
    {"FLUX_z", FieldType::Float},
    {"FLUX_ERROR_z", FieldType::Float},
    {"FLUX_SYSTEM_z", FieldType::Char},
    {"FLUX_BIBCODE_z", FieldType::Char},
    {"FLUX_VAR_z", FieldType::Char},
    {"FLUX_MULT_z", FieldType::Char},
    {"FLUX_QUAL_z", FieldType::Char},
    {"FLUX_UNIT_z", FieldType::Char},
    {"Diameter_diameter", FieldType::Double},
    {"Diameter_Q", FieldType::Char},
    {"Diameter_unit", FieldType::Char},
    {"Diameter_error", FieldType::Double},
    {"Diameter_filter", FieldType::Char},
    {"Diameter_method", FieldType::Char},
    {"Diameter_bibcode", FieldType::Char},
    {"Distance_distance", FieldType::Double},
    {"Distance_Q", FieldType::Char},
    {"Distance_unit", FieldType::Char},
    {"Distance_merr", FieldType::Double},
    {"Distance_perr", FieldType::Double},
    {"Distance_method", FieldType::Char},
    {"Distance_bibcode", FieldType::Char},
    {"Fe_H_Teff", FieldType::Int},
    {"Fe_H_log_g", FieldType::Float},
    {"Fe_H_Fe_H", FieldType::Float},
    {"Fe_H_flag", FieldType::Char},
    {"Fe_H_CompStar", FieldType::Char},
    {"Fe_H_CatNo", FieldType::Char},
    {"Fe_H_bibcode", FieldType::Char},
    {"Herschel_ObsId", FieldType::Char},
    {"Herschel_alpha", FieldType::Char},
    {"Herschel_delta", FieldType::Char},
    {"PLX_plx", FieldType::Float},
    {"PLX_me", FieldType::Float},
    {"PLX_R", FieldType::Char},
    {"PLX_bibcode", FieldType::Char},
    {"PM_pmra", FieldType::Float},
    {"PM_me_pmra", FieldType::Float},
    {"PM_pmde", FieldType::Float},
    {"PM_me_pmde", FieldType::Float},
    {"PM_system", FieldType::Char},
    {"PM_bibcode", FieldType::Char},
    {"ROT_upVsini", FieldType::Char},
    {"ROT_Vsini", FieldType::Float},
    {"ROT_err", FieldType::Float},
    {"ROT_mes", FieldType::Int},
    {"ROT_qual", FieldType::Char},
    {"ROT_bibcode", FieldType::Char},
    {"V__vartyp", FieldType::Char},
    {"V__Lomax", FieldType::Char},
    {"V__max", FieldType::Float},
    {"V__R_max", FieldType::Char},
    {"V__band", FieldType::Char},
    {"V__Upmin", FieldType::Char},
    {"V__min", FieldType::Float},
    {"V__R_min", FieldType::Char},
    {"V__UpPeriod", FieldType::Char},
    {"V__period", FieldType::Double},
    {"V__R_period", FieldType::Char},
    {"V__epoch", FieldType::Double},
    {"V__R_epoch", FieldType::Char},
    {"V__D_rt", FieldType::Float},
    {"V__R_D_rt", FieldType::Char},
    {"V__bibcode", FieldType::Char},
    {"velocities_type", FieldType::Char},
    {"velocities_Value", FieldType::Double},
    {"velocities_R", FieldType::Char},
    {"velocities_me", FieldType::Double},
    {"velocities_Acc", FieldType::Char},
    {"velocities_Nmes", FieldType::Int},
    {"velocities_nat", FieldType::Char},
    {"velocities_Q", FieldType::Char},
    {"velocities_dom", FieldType::Char},
    {"velocities_res", FieldType::Float},
    {"velocities_d", FieldType::Char},
    {"velocities_Date", FieldType::Double},
    {"velocities_Rem", FieldType::Char},
    {"velocities_Origin", FieldType::Char},
    {"velocities_bibcode", FieldType::Char},
    {"ISO_TDT", FieldType::Char},
    {"ISO_alpha", FieldType::Double},
    {"ISO_delta", FieldType::Double},
    {"IUE_Homogenized_Name", FieldType::Char},
    {"IUE_ComplID", FieldType::Char},
    {"IUE_PROG", FieldType::Char},
    {"IUE_CL", FieldType::Int},
    {"IUE_D", FieldType::Char},
    {"IUE_CAM", FieldType::Char},
    {"IUE_IMAGE", FieldType::Int},
    {"IUE_A", FieldType::Char},
    {"IUE_FES", FieldType::Int},
    {"IUE_MD", FieldType::Char},
    {"IUE_ObsDate", FieldType::Char},
    {"IUE_Time", FieldType::Char},
    {"IUE_ExpTim", FieldType::Int},
    {"IUE_m", FieldType::Char},
    {"IUE_CEB", FieldType::Char},
    {"IUE_S", FieldType::Char},
    {"IUE_Comments", FieldType::Char},
    {"IUE_F", FieldType::Char},
    {"IUE_bibcode", FieldType::Char},
    {"XMM_Obsno", FieldType::Char},
    {"OID4", FieldType::Char},
};


// Field definitions from SIMBAD XML files.
class Row {
public:
    std::unordered_map<std::string, std::string> fields;

    // Variant to hold any supported type
    using FieldValue = std::variant<std::string, int, double, float>;

    std::optional<FieldValue> getFieldValue(const std::string& key) const {
        auto it = fields.find(key);
        if (it == fields.end() || it->second.empty())
            return std::nullopt;

        auto typeIt = FIELD_TYPE_MAP.find(key);
        if (typeIt == FIELD_TYPE_MAP.end())
            return it->second; // Default to string if type unknown

        try {
            switch (typeIt->second) {
            case FieldType::Char: // String
            case FieldType::Float:
                return std::stof(it->second);
            case FieldType::Int:
                return std::stoi(it->second);
            case FieldType::Double:
                return std::stod(it->second);
            default:
                return it->second; // Already a string
            }
        }
        catch (...) {
            return std::nullopt;
        };
    };


    static Row fromXMl(rapidxml::xml_node<>* tablerow);
};

class VOTable{
public:
    // TODO: Return signature
    // Bool: Success or failure writing into `tableData`
    // Returns 0 on success, 1 on failure
    bool load(const std::string& filename);
    void printTable();

private:
    


    std::unique_ptr<rapidxml::file<>> xmlFile_;
    std::unique_ptr<rapidxml::xml_document<>> doc_;

    std::vector<Row> tableData;
};

