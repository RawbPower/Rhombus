Scene: Untitled
Entities:
  - Entity: 3549712358
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
        OrthographicSize: 12
        OrthographicNear: -1
        OrthographicFar: 1
      Primary: true
      FixedAspectRatio: false
  - Entity: 3192626519
    TagComponent:
      Tag: Floor
    TransformComponent:
      Position: [0, -1.8229599, 0]
      Rotation: [0, -0, 0]
      Scale: [10, 0.5, 1]
    SpriteRendererComponent:
      Color: [0.264874101, 0.558455408, 0.740196109, 1]
    Rigidbody2DComponent:
      BodyType: Static
      FixedRotation: true
    BoxCollider2DComponent:
      Offset: [0, 0]
      Size: [0.5, 0.5]
      Density: 1
      Friction: 0.5
      Restitution: 0
      RestitutionThreshold: 0.5
  - Entity: 3239553144
    TagComponent:
      Tag: Player
    TransformComponent:
      Position: [0, 0, 0]
      Rotation: [0, 0, 0]
      Scale: [1, 1, 1]
    ScriptComponent:
      ScriptName: Player
    SpriteRendererComponent:
      Color: [0.808823526, 0.253748536, 0.253748536, 1]
    Rigidbody2DComponent:
      BodyType: Dynamic
      FixedRotation: true
    BoxCollider2DComponent:
      Offset: [0, 0]
      Size: [0.5, 0.5]
      Density: 1
      Friction: 0.5
      Restitution: 0
      RestitutionThreshold: 0.5