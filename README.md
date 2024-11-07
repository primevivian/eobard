# Meet Eobard, a split 34 key ortholinear keyboard.
Current Hardware Revision: 1.0

<img src="https://github.com/viniciusbrit/eobard/blob/54abefa3f9322f3dde9a3adc79b1acc81ec4291c/Images/Keyboard.jpg" width="75%">

## Project Description
Eobard is an ortholinear 34 key split keyboard made for my personal needs. Inspired by the [Ferris](https://github.com/pierrechevalier83/ferris).

The columns are spaced out vertically so as to provide the most comfortable typing experience for my hand size. It is designed to be cheap to manufacture and simple to assemble. The single PCB is dual sided, only having to be flipped around for assembly of the other side.

## Project Updates
Coming back to work on this after a few months, life got in the way. I've lost interest in split minimalist keyboards like this after thoroughly using my first prototype since then. These are too limiting for my use case.
I still want to finish this project and polish all that needs to be polished. The board design is done (has been for a while), removing the diodes from the previous version, as the Pico board had enough pins to directly wire each switch, and the OLED screen which was kinda useless when the point is to not look at your keyboard.
My KiCad can't seem to render the new gerber files properly so I can't update the project images just yet.

<details>
<summary>Project Tasks</summary>

- [x] Design Layout. 
- [x] Design PCB & Plate.
- [x] Generate Gerbers.
- [ ] Build Prototype.
- [ ] Write Firmware.
- [ ] Write Build Guide with images.
- [ ] Release First Version.
</details>

## Images (First Prototype)
<img src="https://github.com/viniciusbrit/eobard/blob/54abefa3f9322f3dde9a3adc79b1acc81ec4291c/Images/Front%20and%20Back.jpg" width="75%">
<img src="https://github.com/viniciusbrit/eobard/blob/54abefa3f9322f3dde9a3adc79b1acc81ec4291c/Images/Board-Images/eobard_front.jpg" width="75%">
<img src="https://github.com/viniciusbrit/eobard/blob/54abefa3f9322f3dde9a3adc79b1acc81ec4291c/Images/Board-Images/eobard_back.jpg" width="75%">
<img src="https://github.com/viniciusbrit/eobard/blob/54abefa3f9322f3dde9a3adc79b1acc81ec4291c/Images/Plate-Images/eobard-plate_front.jpg" width="75%">

## Bill of Materials
* Raspberry Pi Pico [x2]
* [PJ-320A 3.5mm Audio Jack [x2]](https://www.lcsc.com/product-detail/Audio-Connectors_Hong-Cheng-HC-PJ-320A_C7501806.html)
* Cherry MX Style Switches [x34]
* 4mm M2 Standoff [x9]
* 3mm M2 Screws [x18]
* 01x20 2.54mm Standard Male Pin Headers [x4] [If Pico doesn't have them pre-soldered]

## Ordering PCBs
Dragging and dropping the provided Gerber zips onto JLCPCB or other manufacturer's websites should work just fine. Make sure the plate PCB is 1mm thick and the main pcb is 1.6mm thick.
