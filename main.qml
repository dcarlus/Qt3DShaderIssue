import QtQuick
import QtQuick.Scene3D
import Qt3D.Core
import Qt3D.Render
import Qt3D.Input
import Qt3D.Extras

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Grid Shader Issue with Qt6.3+")

    Scene3D {
        id: scene3D
        anchors.fill: parent
        focus: true
        aspects: ["input", "logic"]
        cameraAspectRatioMode: Scene3D.AutomaticAspectRatio
        antialiasing: false

        Entity {
            id: sceneRoot

            components: [
                RenderSettings {
                    Viewport {
                        id: root
                        normalizedRect: Qt.rect(0.0, 0.0, 1.0, 1.0)

                        RenderSurfaceSelector {
                            CameraSelector {
                                camera: camera

                                ClearBuffers {
                                    buffers: ClearBuffers.ColorDepthBuffer
                                    clearColor: "#E6E6E6"
                                }
                            }
                        }
                    }
                },
                // Event Source will be set by the Qt3DQuickWindow
                InputSettings { }
            ]

            Camera {
                id: camera
                projectionType: CameraLens.PerspectiveProjection
                fieldOfView: 45
                aspectRatio: 16/9
                nearPlane : 0.01
                farPlane : 1000.
                position: Qt.vector3d(4, 3, 4)
                upVector: Qt.vector3d(0, 1, 0)
                viewCenter: Qt.vector3d(0, 0, 0)
            }

            FirstPersonCameraController {
                camera: camera
            }

            InfiniteGridPlane {
                id: sceneGrid
                camera: camera

                smallestGridColor: "#323232"
                intermediateGridColor: "#353535"
                biggestGridColor: "#373737"
            }

            Entity {
                id: test
                components: [mesh, debugMaterial]

                CuboidMesh {
                    id: mesh
                }

                Material {
                    id: debugMaterial

                    effect: Effect {
                        // OpenGL 4.5
                        property string vertexGL4: "qrc:/Resources/Shaders/SimpleShader/simple.vert"
                        property string fragmentGL4: "qrc:/Resources/Shaders/SimpleShader/simple.frag"

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
                                        name: "renderingStyle"
                                        value: true
                                    }
                                }
                            }
                        ]
                    }
                }
            }
        }
    }
}
