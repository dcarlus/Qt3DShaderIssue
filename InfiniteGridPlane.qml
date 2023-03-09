import Qt3D.Core 2.0
import Qt3D.Render 2.0
import Qt3D.Extras 2.0

import test

Entity {
    property Camera camera
    property real alphaValue: 1.

    required property color smallestGridColor
    required property color intermediateGridColor
    required property color biggestGridColor

    id: gridRoot
    components: [gridMesh, gridMaterial]

    VerticalPlaneGeometry {
        id: gridMesh
        width: 2
        height: 2
    }

    Material {
        id: gridMaterial

        parameters: [
            Parameter {
                name: "camNearPlane"
                value: camera.nearPlane
            },
            Parameter {
                name: "camFarPlane"
                value: camera.farPlane
            },
            Parameter {
                name: "camUp"
                value: camera.upVector.normalized()
            },
            Parameter {
                name: "camPosition"
                value: camera.position
            },
            Parameter {
                name: "globalAlpha"
                value: alphaValue
            },
            Parameter {
                name: "smallestGridColor"
                value: smallestGridColor
            },
            Parameter {
                name: "intermediateGridColor"
                value: intermediateGridColor
            },
            Parameter {
                name: "biggestGridColor"
                value: biggestGridColor
            }
        ]

        effect: Effect {
            // OpenGL 4.5
            property string vertexGL4: "qrc:/Resources/Shaders/InfiniteGrid/infinite_grid.vert"
            property string fragmentGL4: "qrc:/Resources/Shaders/InfiniteGrid/infinite_grid.frag"

            FilterKey {
                id: forward
                name: "renderingStyle"
                value: "forward"
            }

            ShaderProgram {
                id: gl4Shader
                vertexShaderCode: loadSource(parent.vertexGL4)
                fragmentShaderCode: loadSource(parent.fragmentGL4)
            }

            techniques: [
                // OpenGL 4.6
                Technique {
                    filterKeys: [forward]
                    graphicsApiFilter {
                        api: GraphicsApiFilter.OpenGL
                        profile: GraphicsApiFilter.CoreProfile
                        majorVersion: 4
                        minorVersion: 6
                    }
                    renderPasses: RenderPass {
                        shaderProgram: gl4Shader
                        filterKeys: FilterKey {
                            name: "transparentMaterial"
                            value: true
                        }
                        renderStates: [
                            AlphaCoverage {},
                            CullFace {
                                mode: CullFace.NoCulling
                            }
                        ]
                    }
                }
            ]
        }
    }
}
