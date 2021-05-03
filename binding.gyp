{
    "targets": [
        {
            "target_name": "native_ospf",
            "cflags!": [ "-fno-exceptions" ],
            "cflags_cc!": [ "-fno-exceptions" ],
            "sources": [
                "./routing/native_ospf/index.cpp",
                "./routing/native_ospf/router_json_parser.cpp",
                "./routing/ospf.cpp",
                "./routing/Router.cpp",
                "./routing/LSDB.cpp"
            ],
            "include_dirs": [
                "<!@(node -p \"require('node-addon-api').include\")",
                "routing/build/rapidjson-src/include/"
            ],
            "defines": [
                'NAPI_DISABLE_CPP_EXCEPTIONS'
            ]
        }
    ]
}
