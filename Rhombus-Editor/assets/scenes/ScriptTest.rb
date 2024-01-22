Scene: Untitled
Entities:
  - Entity: 1911858132968505047
    TagComponent:
      Tag: Background
    TransformComponent:
      Position: [0, 0, -0.100000001]
      Rotation: [0, 0, 0]
      Scale: [20, 20, 1]
    SpriteRendererComponent:
      Color: [0.911764681, 0.618359387, 0.357554764, 1]
      Texture: assets\textures\Checkerboard.png
  - Entity: 14663160443024796245
    TagComponent:
      Tag: Floor
    TransformComponent:
      Position: [0, -2.24886513, 0]
      Rotation: [0, -0, 0]
      Scale: [10, 0.5, 1]
    SpriteRendererComponent:
      Color: [0.686274529, 0.322952688, 0.643530965, 1]
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
  - Entity: 14998261365604727681
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
        OrthographicSize: 14
        OrthographicNear: -1
        OrthographicFar: 1
      Primary: true
      FixedAspectRatio: false
  - Entity: 14173524867793997944
    TagComponent:
      Tag: Player
    TransformComponent:
      Position: [0, 0.846256852, 0]
      Rotation: [0, -0, 0]
      Scale: [1, 1, 1]
    ScriptComponent:
      ScriptName: Player
    SpriteRendererComponent:
      Color: [0.0824682787, 0.646405756, 0.700980425, 1]
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