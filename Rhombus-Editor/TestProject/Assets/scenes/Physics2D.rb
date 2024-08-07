Scene: Untitled
Entities:
  - Entity: 9376946716844081853
    TagComponent:
      Tag: Sprite 2
    TransformComponent:
      Position: [1.90267682, -0.921346664, 0]
      Rotation: [0, -0, 0]
      Scale: [0.99999994, 0.99999994, 1]
    SpriteRendererComponent:
      Color: [0.838235259, 0.361591667, 0.361591667, 1]
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
  - Entity: 16999953785449067982
    TagComponent:
      Tag: Camera
    TransformComponent:
      Position: [0, 0, 1]
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
  - Entity: 7988186794031035410
    TagComponent:
      Tag: Floor
    TransformComponent:
      Position: [0, -2, 0]
      Rotation: [0, -0, 0]
      Scale: [7, 0.652974963, 1]
    SpriteRendererComponent:
      Color: [0.256521821, 0.730392158, 0.193339109, 1]
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
  - Entity: 4276615385203642727
    TagComponent:
      Tag: Sprite
    TransformComponent:
      Position: [-1.70669341, 0.141008988, 0]
      Rotation: [0, -0, 0.558835864]
      Scale: [0.99999994, 0.99999994, 1]
    SpriteRendererComponent:
      Color: [0.740196109, 0.45846054, 0.101595536, 1]
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
  - Entity: 17383758313798007442
    TagComponent:
      Tag: Circle
    TransformComponent:
      Position: [1.7826004, 0.721580505, 0]
      Rotation: [0, -0, 0]
      Scale: [1, 1, 1]
    CircleRendererComponent:
      Color: [0.163446769, 0.457448632, 0.877451003, 1]
      Thickness: 1
      Fade: 0
    Rigidbody2DComponent:
      BodyType: Dynamic
      FixedRotation: false
    CircleCollider2DComponent:
      Offset: [0, 0]
      Radius: 0.5
      Density: 1
      Friction: 0.5
      Restitution: 0
      RestitutionThreshold: 0.5