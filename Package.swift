// swift-tools-version:6.0

import PackageDescription

let package = Package(
    name: "ImageUtil",
    platforms: [
        .macOS(.v14),
        .iOS(.v16)
    ],
    products: [
        // Define the Swift executable
        .executable(
            name: "Run",
            targets: ["Run"]
        )
    ],
    targets: [
        // The Swift executable target
        .executableTarget(
            name: "Run",
			dependencies: ["imgutil"],
			swiftSettings: [.interoperabilityMode(.Cxx)]
        ),
        // The C++ library target for tensors
        .target(
            name: "imgutil",
            dependencies: [],
            path: "imgutil",
            publicHeadersPath: "include",
            cxxSettings: [
                .headerSearchPath("include"),
                .define("USE_CPP_LIB")
            ]
        ),
        // Unit test target (optional)
        .testTarget(
            name: "ImageUtilTests",
            dependencies: ["imgutil"],
			swiftSettings: [.interoperabilityMode(.Cxx)]
        )
    ],
    cxxLanguageStandard: .cxx17
)
