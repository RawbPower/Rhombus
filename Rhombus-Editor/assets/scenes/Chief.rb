Scene: Untitled
Entities:
  - Entity: 6907134579260574627
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
  - Entity: 10635067066317695875
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
      Primary: true
      FixedAspectRatio: false
  - Entity: 6529422571402075691
    TagComponent:
      Tag: Chief
    TransformComponent:
      Position: [0.476757944, 2.00911784, 0.233549833]
      Rotation: [0.0511592478, -0.00950282998, -0.00478666183]
      Scale: [0.999985576, 1.00000012, 0.999999762]
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