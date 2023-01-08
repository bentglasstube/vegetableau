package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "vegetableau",
    data = ["//content"],
    linkopts = [
        "-lSDL2",
        "-lSDL2_image",
        "-lSDL2_mixer",
    ],
    srcs = ["main.cc"],
    deps = [
        "@libgam//:game",
        ":screens",
    ],
)

cc_library(
    name = "screens",
    srcs = [
        "game_screen.cc",
        "title_screen.cc",
        "tutorial_screen.cc",
    ],
    hdrs = [
        "game_screen.h",
        "title_screen.h",
        "tutorial_screen.h",
    ],
    deps = [
        "@libgam//:backdrop",
        "@libgam//:screen",
        "@libgam//:sprite",
        "@libgam//:text",
        ":garden",
        ":moving_text",
    ],
)

cc_library(
    name = "garden",
    srcs = ["garden.cc"],
    hdrs = ["garden.h"],
    deps = [
        "@libgam//:graphics",
        "@libgam//:spritemap",
    ],
)

cc_library(
    name = "moving_text",
    srcs = ["moving_text.cc"],
    hdrs = ["moving_text.h"],
    deps = [
        "@libgam//:graphics",
        "@libgam//:text",
    ],
)
