Scene: Untitled
Entities:
  - Entity: 4988876918826018370
    TagComponent:
      Tag: Face
    TransformComponent:
      Position: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
      Texture: textures\SpriteFace.png
  - Entity: 7419228167392864260
    TagComponent:
      Tag: Pixel Test
    TransformComponent:
      Position: [0, 0, -1]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
      Texture: textures\160x144_GB.png
  - Entity: 3119260235643492496
    TagComponent:
      Tag: Camera
    TransformComponent:
      Position: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 0
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 1000
        OrthographicSize: 350
        OrthographicNear: -1
        OrthographicFar: 1
        PixelPerfect: true
      Primary: true
      FixedAspectRatio: false
    ScriptComponent:
      ScriptName: Camera