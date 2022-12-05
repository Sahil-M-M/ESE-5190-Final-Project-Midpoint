# ESE-5190-Final-Project-Midpoint

# Code:

We modified the code for TFT Display and were able to make a fighter jet and 5 missiles moving towards it. The Fighter jet is controlled using a joystick to dodge the missiles. As the display screen was small and there was a delay in its execution. We modified the VGA example code but there was an error in the color display. When we give it black color it was still red. We try to resolve this by changing the VGA Displays but the error still occurred a different color and a different color was displayed. But if we hold the connects then the error doesn't occur. So we hope to configure this error caused due to loose connection using a VGA Adapter. The code for TFT Display is attached.

## Working Code:

### TFT Display working code:

We have created simulation of missiles and fighter jet. The fighter jet moves as per the input given by the user using joystick.

### VGA Display working code:

We have created simulation of missiles and fighter jet. 

## In-progress Code:

### TFT Display in-progress code:

We are now trying to predict future locations of missiles and findout safe path for fighter jet to escape.

### VGA Display in-progress  code:

We have added missile shapes and currently working on the shape & motion of fighter jet. We are also trying to get the fighter jet moving as per user input using joystick.

## Libraries:

### TFT Display library:

We have modified the libray and added functions to create fighter jet and missiles that take x co-ordinates, y co-ordinates, and color as input. The libray was available for 80x160 pixel display. So we modified it to work for 128x160 pixel display.

## Reference:

### VGA Display:

https://breatharian.eu/hw/picovga/index_en.html

### TFT Display:

https://github.com/bablokb/pico-st7735

https://github.com/elehobica/pico_st7735_80x160


# Media:

## Material:

### RP Pico:

![image](https://user-images.githubusercontent.com/73771085/205670599-ca4d459c-68bf-47e6-99b1-4c5b87ec2c2f.png)

### VGA Cable:

![image](https://user-images.githubusercontent.com/73771085/205670752-6ea57d60-5ac0-452e-ad2d-722c7acc3e40.png)

### TFT Display:

![image](https://user-images.githubusercontent.com/73771085/205670947-48d818ee-7c5a-469f-a02d-dc45f3a6d882.png)

## Troubleshooting:

Here we have attached a GIF showing that we expect the red blocks to be overlapped with blcak ones to create an illusion that the blocks are moving. But it is still showing Red blocks throughout the loop while trying to use VGA Display.

https://user-images.githubusercontent.com/73771085/205679662-0d8bb6a2-115e-45ba-845c-08d28bcbd5fb.mp4

We were able to figure out that this is due to loose connections and it can be overcome using a VGA Adapter.

## Demos:

### Simulation on TFT Display:

https://user-images.githubusercontent.com/73771085/205679577-25e75ea6-5d83-485d-864f-718360222224.mp4

# Block Diagram:

![504d0ffe-2a13-4e42-bfa0-920b9efb4c61](https://user-images.githubusercontent.com/73771085/205677624-b4800647-1903-4544-be2e-738b69b67c88.jpg)

