# OmniWalk | Arbitrary Gravity Framework for UE5

![Platform](https://img.shields.io/badge/Platform-UE5.4%2B-blue.svg) 
![Language](https://img.shields.io/badge/Language-C%2B%2B-orange.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)

**OmniWalk** is a high-performance C++ middleware framework designed to solve the technical "Showstoppers" of non-Z-up locomotion in Unreal Engine. It delivers a "Zero-Config" solution for Ratchet & Clank style surface adhesion on arbitrary meshes.

## 🚀 Key Technical USPs

- **Native 5.4+ Integration:** Deeply integrated with `UCharacterMovementComponent::SetGravityDirection` for frame-perfect physics integration.
- **The "Singular Component":** No-code setup. Automatically hijacks Pawn settings, stabilizes camera gimbals, and remaps input vectors to surface planes.
- **Gimbal-Free Solver:** Custom camera modifier logic eliminates control inversion and view-locking at ±90° pitch.
- **Surface-Projected Input:** Intercepts and re-projects movement inputs onto triangle normals to prevent "capsule pinning" against vertical walls.
- **Slate Telemetry:** Dedicated editor debugger for real-time visualization of gravity vectors and alignment quality.

## 📁 Repository Structure
````
OmniWalk/
├── Source/
│   ├── OmniWalk/       # Runtime Module (Adhesion & Input Hijacking)
│   └── OmniWalkEditor/ # Editor Module (Slate UI & Telemetry)
├── Resources/          # Icons and Visual Assets
├── Content/            # UI Styles and Prototype Blueprints
└── OmniWalk.uplugin    # Descriptor# OmniWalk
Zero-Config Arbitrary Gravity &amp; Surface Adhesion Framework for UE5.4+. Native C++ "Magneboot" locomotion with gimbal-free camera stabilization.
````

## 🛠 Usage (Zero-Config Path)

  Add Component: Attach UOmniWalkPro to any ACharacter.

  Tag (Optional): Add actor tag OmniWalk.Enabled for subsystem auto-injection.

  Play: Move toward any surface. The framework handles orientation, gravity redefinition, and camera stabilization automatically.

## 🧠 Architectural Insights

OmniWalk ticks in the TG_PrePhysics group, ensuring that the redefined gravity vector is utilized by the CharacterMovementComponent during the current frame's integration. It utilizes Slerp-normalized Quaternions for orientation to avoid Euler singularities inherent in traditional platformer movement.

## 📄 License

Distributed under the MIT License. See LICENSE for more information.
