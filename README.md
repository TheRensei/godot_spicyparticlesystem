
<p align="center">
<img src="https://github.com/TheRensei/godot_spicyparticlesystem/blob/main/addons/spicyparticlesystem/icons/NodeIcon.svg" width="100" height="100"/>
</p>
<h1 align="center">Spicy Particle System</h1>
<p align="center">
  <a href="https://github.com/TheRensei/godot_spicyparticlesystem/releases">
    <img alt="GitHub all releases" src="https://img.shields.io/github/downloads/TheRensei/godot_spicyparticlesystem/total">
  </a>
  <a href="https://github.com/TheRensei/godot_spicyparticlesystem/releases/latest">
    <img alt="GitHub release (with filter)" src="https://img.shields.io/github/v/release/TheRensei/godot_spicyparticlesystem?filter=beta">
  </a>
  <a href="LICENSE">
    <img src="https://img.shields.io/github/license/TheRensei/godot_spicyparticlesystem?style=flat-square" alt="LICENSE">
  </a>
  <a href="https://github.com/TheRensei/godot_spicyparticlesystem/wiki">
    <img alt="Static Badge" src="https://img.shields.io/badge/wiki-blue">
  </a>
</p>

> [!WARNING]  
> This extension is not production ready yet. Backup your projects before testing!

<h2 align="center">Introduction</h2>

<img align="left" src="/.images/Main.png">


Spicy Particle System is a powerful and customizable particle system written in C++ as a GDExtension for the Godot Engine. This system is designed to offer improved performance compared to the existing CPUParticles3D while providing extensive customization options for your VFX.

## Features

- **Modular Design:** Features a modular design with modules divided into generators and updaters. Generators define starting particle parameters, while updaters dynamically update particle properties over their lifetime.
> [!NOTE]  
> Modules are Resources; only assigned modules are executed. Consider the impact on performance when utilizing multiple modules.
- **3D transforms:** Particles are transformed in three dimensions, providing the flexibility to customize size, rotation, velocity, and more with full control over each dimension.
- **On-Screen Editor:** The intuitive on-screen editor allows control of playback and seeking but also displays real-time information on the number of alive particles within the selected system.
- **Random Seed Control:** Take command of randomness with precise control over the random seed, allowing for reproducible and fine-tuned variations in your particle system.
- **Emit Rate Over Distance:** Dynamically spawn particles based on the distance traveled.
- **Burst Emission:** Shedule particles to be emitted at specific time.
- **CPU Alignment:** While shader alignment is favored for its efficiency, the system also includes the option of CPU particle alignment.
- **Hierarchical playback:** Hierarchical playback synchronizes children systems' playback with the parent system if the direct parent is also a particle system. This feature allows for intricate control over particle system behavior in complex hierarchies.


<br clear="left"/>

---

</details>

<details><summary> <h3>  References </h3> </summary>
  
Big thanks to Bartlomiej Filipek for his [blog post](https://www.cppstories.com/2014/04/flexible-particle-system-start/) - it was extremely helpful and helped me understand how particle systems are written in the first place. I thought that this approach was a great place to start with so this is what this system is based on.

Links to icons used:

[Link](https://www.svgrepo.com/svg/320810/flaming-claw)
[Link](https://www.svgrepo.com/svg/525367/heart-shine)
[Link](https://www.svgrepo.com/svg/533001/square-dashed)
[Link](https://www.svgrepo.com/svg/458610/color-mode)
[Link](https://www.svgrepo.com/svg/489117/scale-1)
[Link](https://www.svgrepo.com/svg/374909/high-velocity-sales)
[Link](https://www.svgrepo.com/svg/375107/rotate)
[Link](https://www.svgrepo.com/svg/375045/product-transfer)
[Link](https://www.svgrepo.com/svg/502881/update)
[Link](https://www.svgrepo.com/svg/502459/alarm)

I've resized, optimized and coloured most of them, the main icon is a combination of 2 other icons.
With [CC](https://www.svgrepo.com/page/licensing/#CC%20Attribution) and PD licenses.

</details>
