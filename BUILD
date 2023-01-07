package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "ld52",
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
    ],
    hdrs = [
        "game_screen.h",
        "title_screen.h",
    ],
    deps = [
        "@libgam//:backdrop",
        "@libgam//:screen",
        "@libgam//:sprite",
        "@libgam//:text",
        ":garden",
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
