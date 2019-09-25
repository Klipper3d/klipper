# coding: utf-8
from __future__ import unicode_literals

import mock
import pytest

from extras.bltouch import FlavoredConfig


def test_default_flavor_value():
    """
    Ensure we provide "default" as the default value when calling the
    underlying getchoice("flavor", ...)
    Ensure the returned value is stored in `flavor` attribute.
    """
    config_wrapper = mock.Mock(getchoice=mock.Mock(return_value="some_flavor"))

    flavored = FlavoredConfig(config_wrapper)

    config_wrapper.getchoice.assert_called_once_with(
        "flavor", mock.ANY, default="default"
    )
    assert flavored.flavor == "some_flavor"


def test_non_flavored_options():
    """
    Non flavored options are handled transparently.
      - if a default value is provided, we pass it to the underlying ConfigWrapper
      - if no default value is provided, we pass no default value to the
        underlying ConfigWrapper
    """
    config_wrapper = mock.Mock(
        getchoice=mock.Mock(return_value="some_flavor"),
        get=mock.Mock(return_value="bar"),
        getboolean=mock.Mock(return_value=True),
        getfloat=mock.Mock(return_value=1.234),
    )
    flavored = FlavoredConfig(config_wrapper)

    #
    # with default
    #
    assert flavored.getboolean("foo", default=False) is True
    config_wrapper.getboolean.assert_called_once_with("foo", default=False)

    assert flavored.get("foo", default="klipper") == "bar"
    config_wrapper.get.assert_called_once_with("foo", default="klipper")

    assert flavored.getfloat("foo", default=2.345) == 1.234
    config_wrapper.getfloat.assert_called_once_with("foo", default=2.345)

    #
    # without default
    #
    config_wrapper.reset_mock()

    assert flavored.getboolean("foo") is True
    config_wrapper.getboolean.assert_called_once_with("foo")

    assert flavored.get("foo") == "bar"
    config_wrapper.get.assert_called_once_with("foo")

    assert flavored.getfloat("foo") == 1.234
    config_wrapper.getfloat.assert_called_once_with("foo")


def test_flavored_options():
    """
    Flavored options are handled as follow.
      - if a default value is provided, we pass it to the underlying ConfigWrapper
      - if no default value is provided:
        - if the current flavor defines a default, pass it to the ConfigWrapper
        - otherwise, pass no default value
    """
    config_wrapper = mock.Mock(
        getchoice=mock.Mock(return_value="some_flavor"),
        getboolean=mock.Mock(return_value=mock.sentinel.config_value),
    )
    flavored = FlavoredConfig(config_wrapper)
    flavored.defaults["flavored_value"] = mock.sentinel.flavored_value

    #
    # with default
    #
    assert (
        flavored.getboolean("flavored_value", default=False)
        == mock.sentinel.config_value
    )
    config_wrapper.getboolean.assert_called_once_with("flavored_value", default=False)

    #
    # without default
    #
    config_wrapper.reset_mock()

    assert flavored.getboolean("flavored_value") == mock.sentinel.config_value
    config_wrapper.getboolean.assert_called_once_with(
        "flavored_value", default=mock.sentinel.flavored_value
    )


def test_backward_compatible_default_settings():
    """
    When no flavor is provided, we provide backward compatible defaults.
    """
    config_wrapper = mock.Mock(getchoice=mock.Mock(return_value="default"))
    flavored = FlavoredConfig(config_wrapper)
    assert flavored.defaults["pin_up_reports_not_triggered"] is True
    assert flavored.defaults["pin_up_touch_mode_reports_triggered"] is True
    assert flavored.defaults["keep_signal_during_probe"] is False
    assert flavored.defaults["probe_with_touch_switch_mode"] is False


expected_default_values = [
    # pin_up_reports_not_triggered
    ("default", "pin_up_reports_not_triggered", True),
    ("genuine_classic_1.0", "pin_up_reports_not_triggered", True),
    ("genuine_classic_1.1", "pin_up_reports_not_triggered", True),
    ("genuine_classic_1.2", "pin_up_reports_not_triggered", True),
    ("genuine_classic_1.3", "pin_up_reports_not_triggered", True),
    ("genuine_smart_1.0", "pin_up_reports_not_triggered", True),
    ("genuine_smart_2.0", "pin_up_reports_not_triggered", True),
    ("genuine_smart_2.1", "pin_up_reports_not_triggered", True),
    ("genuine_smart_2.2", "pin_up_reports_not_triggered", True),
    ("genuine_smart_3.0", "pin_up_reports_not_triggered", True),
    ("genuine_smart_3.1", "pin_up_reports_not_triggered", True),
    # pin_up_touch_mode_reports_triggered
    ("default", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_classic_1.0", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_classic_1.1", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_classic_1.2", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_classic_1.3", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_smart_1.0", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_smart_2.0", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_smart_2.1", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_smart_2.2", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_smart_3.0", "pin_up_touch_mode_reports_triggered", True),
    ("genuine_smart_3.1", "pin_up_touch_mode_reports_triggered", True),
    # keep_signal_during_probe
    ("default", "keep_signal_during_probe", False),
    ("genuine_classic_1.0", "keep_signal_during_probe", True),
    ("genuine_classic_1.1", "keep_signal_during_probe", True),
    ("genuine_classic_1.2", "keep_signal_during_probe", True),
    ("genuine_classic_1.3", "keep_signal_during_probe", True),
    ("genuine_smart_1.0", "keep_signal_during_probe", True),
    ("genuine_smart_2.0", "keep_signal_during_probe", True),
    ("genuine_smart_2.1", "keep_signal_during_probe", True),
    ("genuine_smart_2.2", "keep_signal_during_probe", True),
    ("genuine_smart_3.0", "keep_signal_during_probe", True),
    ("genuine_smart_3.1", "keep_signal_during_probe", True),
    # probe_with_touch_switch_mode
    ("default", "probe_with_touch_switch_mode", False),
    ("genuine_classic_1.0", "probe_with_touch_switch_mode", False),
    ("genuine_classic_1.1", "probe_with_touch_switch_mode", False),
    ("genuine_classic_1.2", "probe_with_touch_switch_mode", False),
    ("genuine_classic_1.3", "probe_with_touch_switch_mode", False),
    ("genuine_smart_1.0", "probe_with_touch_switch_mode", False),
    ("genuine_smart_2.0", "probe_with_touch_switch_mode", False),
    ("genuine_smart_2.1", "probe_with_touch_switch_mode", True),
    ("genuine_smart_2.2", "probe_with_touch_switch_mode", True),
    ("genuine_smart_3.0", "probe_with_touch_switch_mode", True),
    ("genuine_smart_3.1", "probe_with_touch_switch_mode", True),
]


@pytest.mark.parametrize("flavor, option, expected", expected_default_values)
def test_default_values(flavor, option, expected):
    config_wrapper = mock.Mock(getchoice=mock.Mock(return_value=flavor))
    flavored = FlavoredConfig(config_wrapper)
    assert flavored.defaults[option] == expected
