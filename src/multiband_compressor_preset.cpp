/*
 *  Copyright © 2017-2022 Wellington Wallace
 *
 *  This file is part of EasyEffects.
 *
 *  EasyEffects is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  EasyEffects is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with EasyEffects.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "multiband_compressor_preset.hpp"

MultibandCompressorPreset::MultibandCompressorPreset(PresetType preset_type, const int& index)
    : PluginPresetBase(tags::schema::multiband_compressor::id,
                       tags::schema::multiband_compressor::input_path,
                       tags::schema::multiband_compressor::output_path,
                       preset_type,
                       index) {}

void MultibandCompressorPreset::save(nlohmann::json& json) {
  json[section]["multiband_compressor"]["bypass"] = g_settings_get_boolean(settings, "bypass") != 0;

  json[section]["multiband_compressor"]["input-gain"] = g_settings_get_double(settings, "input-gain");

  json[section]["multiband_compressor"]["output-gain"] = g_settings_get_double(settings, "output-gain");

  json[section]["multiband_compressor"]["dry"] = g_settings_get_double(settings, "dry");

  json[section]["multiband_compressor"]["wet"] = g_settings_get_double(settings, "wet");

  json[section]["multiband_compressor"]["compressor-mode"] = util::gsettings_get_string(settings, "compressor-mode");

  json[section]["multiband_compressor"]["envelope-boost"] = util::gsettings_get_string(settings, "envelope-boost");

  for (uint n = 0U; n < n_bands; n++) {
    const auto nstr = util::to_string(n);
    const auto bandn = "band" + nstr;

    if (n > 0U) {
      json[section]["multiband_compressor"][bandn]["enable-band"] =
          g_settings_get_boolean(settings, ("enable-band" + nstr).c_str()) != 0;

      json[section]["multiband_compressor"][bandn]["split-frequency"] =
          g_settings_get_double(settings, ("split-frequency" + nstr).c_str());
    }

    json[section]["multiband_compressor"][bandn]["compressor-enable"] =
        g_settings_get_boolean(settings, ("compressor-enable" + nstr).c_str()) != 0;

    json[section]["multiband_compressor"][bandn]["solo"] =
        g_settings_get_boolean(settings, ("solo" + nstr).c_str()) != 0;

    json[section]["multiband_compressor"][bandn]["mute"] =
        g_settings_get_boolean(settings, ("mute" + nstr).c_str()) != 0;

    json[section]["multiband_compressor"][bandn]["attack-threshold"] =
        g_settings_get_double(settings, ("attack-threshold" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["attack-time"] =
        g_settings_get_double(settings, ("attack-time" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["release-threshold"] =
        g_settings_get_double(settings, ("release-threshold" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["release-time"] =
        g_settings_get_double(settings, ("release-time" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["ratio"] = g_settings_get_double(settings, ("ratio" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["knee"] = g_settings_get_double(settings, ("knee" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["makeup"] = g_settings_get_double(settings, ("makeup" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["compression-mode"] =
        util::gsettings_get_string(settings, ("compression-mode" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["external-sidechain"] =
        g_settings_get_boolean(settings, ("external-sidechain" + nstr).c_str()) != 0;

    json[section]["multiband_compressor"][bandn]["sidechain-mode"] =
        util::gsettings_get_string(settings, ("sidechain-mode" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["sidechain-source"] =
        util::gsettings_get_string(settings, ("sidechain-source" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["sidechain-lookahead"] =
        g_settings_get_double(settings, ("sidechain-lookahead" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["sidechain-reactivity"] =
        g_settings_get_double(settings, ("sidechain-reactivity" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["sidechain-preamp"] =
        g_settings_get_double(settings, ("sidechain-preamp" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["sidechain-custom-lowcut-filter"] =
        g_settings_get_boolean(settings, ("sidechain-custom-lowcut-filter" + nstr).c_str()) != 0;

    json[section]["multiband_compressor"][bandn]["sidechain-custom-highcut-filter"] =
        g_settings_get_boolean(settings, ("sidechain-custom-highcut-filter" + nstr).c_str()) != 0;

    json[section]["multiband_compressor"][bandn]["sidechain-lowcut-frequency"] =
        g_settings_get_double(settings, ("sidechain-lowcut-frequency" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["sidechain-highcut-frequency"] =
        g_settings_get_double(settings, ("sidechain-highcut-frequency" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["boost-threshold"] =
        g_settings_get_double(settings, ("boost-threshold" + nstr).c_str());

    json[section]["multiband_compressor"][bandn]["boost-amount"] =
        g_settings_get_double(settings, ("boost-amount" + nstr).c_str());
  }
}

void MultibandCompressorPreset::load(const nlohmann::json& json) {
  update_key<bool>(json.at(section).at("multiband_compressor"), settings, "bypass", "bypass");

  update_key<double>(json.at(section).at("multiband_compressor"), settings, "input-gain", "input-gain");

  update_key<double>(json.at(section).at("multiband_compressor"), settings, "output-gain", "output-gain");

  update_key<double>(json.at(section).at("multiband_compressor"), settings, "dry", "dry");

  update_key<double>(json.at(section).at("multiband_compressor"), settings, "wet", "wet");

  update_key<gchar*>(json.at(section).at("multiband_compressor"), settings, "compressor-mode", "compressor-mode");

  update_key<gchar*>(json.at(section).at("multiband_compressor"), settings, "envelope-boost", "envelope-boost");

  for (uint n = 0U; n < n_bands; n++) {
    const auto nstr = util::to_string(n);
    const auto bandn = "band" + nstr;

    if (n > 0U) {
      update_key<bool>(json.at(section).at("multiband_compressor").at(bandn), settings, "enable-band" + nstr,
                       "enable-band");

      update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "split-frequency" + nstr,
                         "split-frequency");
    }

    update_key<bool>(json.at(section).at("multiband_compressor").at(bandn), settings, "compressor-enable" + nstr,
                     "compressor-enable");

    update_key<bool>(json.at(section).at("multiband_compressor").at(bandn), settings, "solo" + nstr, "solo");

    update_key<bool>(json.at(section).at("multiband_compressor").at(bandn), settings, "mute" + nstr, "mute");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "attack-threshold" + nstr,
                       "attack-threshold");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "attack-time" + nstr,
                       "attack-time");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "release-threshold" + nstr,
                       "release-threshold");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "release-time" + nstr,
                       "release-time");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "ratio" + nstr, "ratio");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "knee" + nstr, "knee");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "makeup" + nstr, "makeup");

    update_key<gchar*>(json.at(section).at("multiband_compressor").at(bandn), settings, "compression-mode" + nstr,
                       "compression-mode");

    update_key<bool>(json.at(section).at("multiband_compressor").at(bandn), settings, "external-sidechain" + nstr,
                     "external-sidechain");

    update_key<gchar*>(json.at(section).at("multiband_compressor").at(bandn), settings, "sidechain-mode" + nstr,
                       "sidechain-mode");

    update_key<gchar*>(json.at(section).at("multiband_compressor").at(bandn), settings, "sidechain-source" + nstr,
                       "sidechain-source");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "sidechain-lookahead" + nstr,
                       "sidechain-lookahead");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "sidechain-reactivity" + nstr,
                       "sidechain-reactivity");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "sidechain-preamp" + nstr,
                       "sidechain-preamp");

    update_key<bool>(json.at(section).at("multiband_compressor").at(bandn), settings,
                     "sidechain-custom-lowcut-filter" + nstr, "sidechain-custom-lowcut-filter");

    update_key<bool>(json.at(section).at("multiband_compressor").at(bandn), settings,
                     "sidechain-custom-highcut-filter" + nstr, "sidechain-custom-highcut-filter");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings,
                       "sidechain-lowcut-frequency" + nstr, "sidechain-lowcut-frequency");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings,
                       "sidechain-highcut-frequency" + nstr, "sidechain-highcut-frequency");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "boost-threshold" + nstr,
                       "boost-threshold");

    update_key<double>(json.at(section).at("multiband_compressor").at(bandn), settings, "boost-amount" + nstr,
                       "boost-amount");
  }
}
