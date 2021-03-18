/*
 *  Copyright © 2017-2020 Wellington Wallace
 *
 *  This file is part of PulseEffects.
 *
 *  PulseEffects is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  PulseEffects is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with PulseEffects.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "stereo_tools_ui.hpp"
#include <cstring>

namespace {

auto stereo_tools_enum_to_int(GValue* value, GVariant* variant, gpointer user_data) -> gboolean {
  const auto* v = g_variant_get_string(variant, nullptr);

  if (std::strcmp(v, "LR > LR (Stereo Default)") == 0) {
    g_value_set_int(value, 0);
  } else if (std::strcmp(v, "LR > MS (Stereo to Mid-Side)") == 0) {
    g_value_set_int(value, 1);
  } else if (std::strcmp(v, "MS > LR (Mid-Side to Stereo)") == 0) {
    g_value_set_int(value, 2);
  } else if (std::strcmp(v, "LR > LL (Mono Left Channel)") == 0) {
    g_value_set_int(value, 3);
  } else if (std::strcmp(v, "LR > RR (Mono Right Channel)") == 0) {
    g_value_set_int(value, 4);
  } else if (std::strcmp(v, "LR > L+R (Mono Sum L+R)") == 0) {
    g_value_set_int(value, 5);
  } else if (std::strcmp(v, "LR > RL (Stereo Flip Channels)") == 0) {
    g_value_set_int(value, 6);
  }

  return 1;
}

auto int_to_stereo_tools_enum(const GValue* value, const GVariantType* expected_type, gpointer user_data) -> GVariant* {
  const auto v = g_value_get_int(value);

  switch (v) {
    case 0:
      return g_variant_new_string("LR > LR (Stereo Default)");

    case 1:
      return g_variant_new_string("LR > MS (Stereo to Mid-Side)");

    case 2:
      return g_variant_new_string("MS > LR (Mid-Side to Stereo)");

    case 3:
      return g_variant_new_string("LR > LL (Mono Left Channel)");

    case 4:
      return g_variant_new_string("LR > RR (Mono Right Channel)");

    case 5:
      return g_variant_new_string("LR > L+R (Mono Sum L+R)");

    case 6:
      return g_variant_new_string("LR > RL (Stereo Flip Channels)");

    default:
      return g_variant_new_string("LR > LR (Stereo Default)");
  }
}

}  // namespace

StereoToolsUi::StereoToolsUi(BaseObjectType* cobject,
                             const Glib::RefPtr<Gtk::Builder>& builder,
                             const std::string& schema,
                             const std::string& schema_path)
    : Gtk::Box(cobject), PluginUiBase(builder, schema, schema_path) {
  name = "stereo_tools";

  // loading glade widgets

  input_gain = builder->get_widget<Gtk::Scale>("input_gain");
  output_gain = builder->get_widget<Gtk::Scale>("output_gain");
  balance_in = builder->get_widget<Gtk::SpinButton>("balance_in");
  balance_out = builder->get_widget<Gtk::SpinButton>("balance_out");
  slev = builder->get_widget<Gtk::SpinButton>("slev");
  sbal = builder->get_widget<Gtk::SpinButton>("sbal");
  mlev = builder->get_widget<Gtk::SpinButton>("mlev");
  mpan = builder->get_widget<Gtk::SpinButton>("mpan");
  stereo_base = builder->get_widget<Gtk::SpinButton>("stereo_base");
  delay = builder->get_widget<Gtk::SpinButton>("delay");
  sc_level = builder->get_widget<Gtk::SpinButton>("sc_level");
  stereo_phase = builder->get_widget<Gtk::SpinButton>("stereo_phase");
  softclip = builder->get_widget<Gtk::ToggleButton>("softclip");
  mutel = builder->get_widget<Gtk::ToggleButton>("mutel");
  muter = builder->get_widget<Gtk::ToggleButton>("muter");
  phasel = builder->get_widget<Gtk::ToggleButton>("phasel");
  phaser = builder->get_widget<Gtk::ToggleButton>("phaser");
  mode = builder->get_widget<Gtk::ComboBoxText>("mode");

  reset_button = builder->get_widget<Gtk::Button>("reset_button");

  // gsettings bindings

  settings->bind("installed", this, "sensitive", flag);

  settings->bind("softclip", softclip, "active", flag);
  settings->bind("mutel", mutel, "active", flag);
  settings->bind("muter", muter, "active", flag);
  settings->bind("phasel", phasel, "active", flag);
  settings->bind("phaser", phaser, "active", flag);
  settings->bind("input-gain", input_gain.get(), "value", flag);
  settings->bind("output-gain", output_gain.get(), "value", flag);
  settings->bind("balance-in", balance_in.get(), "value", flag);
  settings->bind("balance-out", balance_out.get(), "value", flag);
  settings->bind("slev", slev.get(), "value", flag);
  settings->bind("sbal", sbal.get(), "value", flag);
  settings->bind("mlev", mlev.get(), "value", flag);
  settings->bind("mpan", mpan.get(), "value", flag);
  settings->bind("stereo-base", stereo_base.get(), "value", flag);
  settings->bind("delay", delay.get(), "value", flag);
  settings->bind("sc-level", sc_level.get(), "value", flag);
  settings->bind("stereo-phase", stereo_phase.get(), "value", flag);

  g_settings_bind_with_mapping(settings->gobj(), "mode", mode->gobj(), "active", G_SETTINGS_BIND_DEFAULT,
                               stereo_tools_enum_to_int, int_to_stereo_tools_enum, nullptr, nullptr);

  reset_button->signal_clicked().connect([=]() { reset(); });
}

StereoToolsUi::~StereoToolsUi() {
  util::debug(name + " ui destroyed");
}

void StereoToolsUi::reset() {
  settings->reset("input-gain");

  settings->reset("output-gain");

  settings->reset("balance-in");

  settings->reset("balance-out");

  settings->reset("softclip");

  settings->reset("mutel");

  settings->reset("muter");

  settings->reset("phasel");

  settings->reset("phaser");

  settings->reset("mode");

  settings->reset("slev");

  settings->reset("sbal");

  settings->reset("mlev");

  settings->reset("mpan");

  settings->reset("stereo-base");

  settings->reset("delay");

  settings->reset("sc-level");

  settings->reset("stereo-phase");
}
