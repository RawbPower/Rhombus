Scene: Untitled
Entities:
  - Entity: 9258455513912969936
    TagComponent:
      Tag: Chief
    TransformComponent:
      Position: [0.473457068, 1.24154675, 0.194248483]
      Rotation: [0.0511592478, -0.00950342324, -0.00478666136]
      Scale: [0.999935031, 1.00000012, 0.999999762]
    SpriteRendererComponent:
      Color: [0.921568632, 0.921568632, 0.262014627, 1]
    Rigidbody2DComponent:
      BodyType: Dynamic
      FixedRotation: false
    BoxCollider2DComponent:
      Offset: [0, 0]
      Size: [0.5, 0.5]
      Density: 1
      Friction: 0.5
      Restitution: 0
      RestitutionThreshold: 0.5
  - Entity: 13441089046500248050
    TagComponent:
      Tag: Chief Cam
    TransformComponent:
      Position: [0, 0, 0]
      Rotation: [0, -0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 0
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 1000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
        PixelPerfect: false
      Primary: true
      FixedAspectRatio: false
  - Entity: 16546543849428095860
    TagComponent:
      Tag: Floor
    TransformComponent:
      Position: [0, -1.50873947, 0]
      Rotation: [0, -0, 0]
      Scale: [7.16815186, 0.611474156, 0.98999995]
    SpriteRendererComponent:
      Color: [0.287389487, 0.594675303, 0.90196079, 1]
    Rigidbody2DComponent:
      BodyType: Static
      FixedRotation: false
    BoxCollider2DComponent:
      Offset: [0, 0]
      Size: [0.5, 0.5]
      Density: 1
      Friction: 0.5
      Restitution: 0
      RestitutionThreshold: 0.5