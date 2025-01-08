-- Copyright lowRISC Contributors.
-- SPDX-License-Identifier: Apache-2.0

set_project("Sonata SPI Examples")
sdkdir = "../../cheriot-rtos/sdk"
includes(sdkdir)
set_toolchains("cheriot-clang")

includes(path.join(sdkdir, "lib"))
--includes("../../libraries")
includes("../../common.lua")

option("board")
    set_default("sonata-prerelease")

compartment("main_comp")
    add_deps("debug")
    add_files("basic_spi.cc")

firmware("basic_spi")
    add_deps("freestanding", "main_comp")
    on_load(function(target)
        target:values_set("board", "$(board)")
        target:values_set("threads", {
            {
                compartment = "main_comp",
                priority = 2,
                entry_point = "main_entry",
                stack_size = 0x300,
                trusted_stack_frames = 1
            },
        }, {expand = false})
    end)
    after_link(convert_to_uf2)

