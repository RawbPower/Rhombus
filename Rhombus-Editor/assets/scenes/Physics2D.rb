Scene: Untitled
Entities:
  - Entity: 6538702533909334857
    TagComponent:
      Tag: Sprite
    TransformComponent:
      Position: [0, 0, 0]
      Rotation: [0, -0, 0.558835864]
      Scale: [0.99999994, 0.99999994, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
    Rigidbody2DComponent:
      BodyType: Dynamic
      FixedRotation: false
    BoxCollider2DComponent:
      Offset: [0, 0]
      Size: [0.5, 0.5]
      Density: 1
      Friction: 0.5
      Restitution: 0.460000008
      RestitutionThreshold: 0.5
  - Entity: 13930855250671538567
    TagComponent:
      Tag: Camera
    TransformComponent:
      Position: [0, 0, 10]
      Rotation: [0, -0, 0]
      Scale: [1, 1, 1]
    CameraComponent:
      Camera:
        ProjectionType: 1
        PerspectiveFOV: 0.785398185
        PerspectiveNear: 0.00999999978
        PerspectiveFar: 1000
        OrthographicSize: 10
        OrthographicNear: -1
        OrthographicFar: 1
      Primary: true
      FixedAspectRatio: false
  - Entity: 5479072972290806098
    TagComponent:
      Tag: Floor
    TransformComponent:
      Position: [0, -2, 0]
      Rotation: [0, -0, 0]
      Scale: [7, 0.652974963, 1]
    SpriteRendererComponent:
      Color: [1, 1, 1, 1]
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