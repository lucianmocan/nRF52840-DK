# TP 2 — 6LoWPAN & RPL (Contiki-NG)

This TP guides you through building UDP communication between two IoT nodes using **6LoWPAN**, analyzing header compression and fragmentation. In addition, it guides you through deploying the **RPL** routing protocol using Contiki-NG. You will build a local RPL network, compare **Non-storing** vs **Storing** modes.
The work is **done in pairs** and uses **nRF52840 DK** boards.

---

### Prerequisites
- Four nRF52840 DK boards.
- Wireshark / Tshark (for packet analysis) and `sensniff` (from Lab 1).

---
## A. 6LoWPAN

### Learning Objectives
- Configure IPv6 communication on a constrained IoT wireless network.
- Understand **6LoWPAN** compression and fragmentation mechanisms.
- Analyze network traces and compute compression ratios.
---



### A.1) UDP Communication

Implement unidirectional UDP between your two boards using Contiki-NG’s LOG system for output.

The documentation of UDP APIs in contiki is available [here](https://docs.contiki-ng.org/en/develop/_api/group__simple-udp.html) with a [basic example](https://docs.contiki-ng.org/en/develop/doc/programming/UDP-communication.html)

**Message payload must include:**
- A **sequence number** (1 byte)
- The **raw temperature sensor value**
- A **text message** (use your family names)

**Work to do:**
1. Choose a **radio channel** (11–26) and a **PAN ID** different from other groups; set them in `project-conf.h`.
2. Configure the stack to use **CSMA** (L2), **IPv6** (L3), and **NULLROUTING** (routing).
3. Select a **sender port** and **receiver port**; set `PORT_SENDER` and `PORT_RECV` in both `sender-udp.c` and `receiver-udp.c`.
4. Create the UDP connection with `simple_udp_register()` in **both** programs using matching parameters.
5. In the sender, transmit **one UDP datagram every 5 seconds** to the receiver. For each transmission, print destination address and message contents. (You may reuse TP 1 code to read the temperature sensor.)
6. Implement the **receive callback** on the receiver to print the **source IP address** and all message fields. Print the **actual temperature** (as in TP 1).
7. Test in both directions (swap sender/receiver) and verify logs on both boards.

---

### A.2) 6LoWPAN Analysis & Optimization

Capture traffic and analyze 6LoWPAN compression.

**Work to do:**
1. Run `sensniff` on a third board to capture sender↔receiver traffic.
2. Record a trace of a **single UDP datagram** from your sender to your partner’s receiver.
   - Note: by default the **FCS** is non-standard and higher-layer headers may not decode in **Tshark** or **Wireshark**.
   - Fix in **Tshark**: `-o "wpan.fcs_format:TI CC24xx metadata"`
   - Fix in **Wireshark**: `Edit > Preferences > Protocols > IEEE 802.15.4 > FCS format: TI CC24xx metadata`
3. Analyze your trace and answer:
   - **Which compression standard** is used by 6LoWPAN?
   - **How is the IPv6 header compressed?** Compute the **compression ratio** (IP layer only).
   - **How is the UDP header compressed?** Compute the **overall compression ratio** (entire message).
4. **Improve compression**: adjust your configuration to increase the compression ratio. Explain and justify the changes. Capture a new trace (in each direction) to validate effectiveness.

---

### A.3) 6LoWPAN Fragmentation

Increase the transmitted message size so it **exceeds the IEEE 802.15.4 MTU**.

**Work to do:**
- Observe the changes in the **6LoWPAN headers** related to fragmentation.
- Record a **complete trace** of sending one UDP datagram in this scenario (**one trace per direction**).

---
## B. RPL Protocol

You can find the documentation of RPL protocol in contiki [here](https://docs.contiki-ng.org/en/develop/doc/programming/RPL.html).

### Learning Objectives
- Set up a local RPL network for constrained wireless IoT devices.
- Understand the differences between **Storing** and **Non-storing** modes in RPL.

---


## B.1) RPL Non-storing Mode

Build a **Non-storing** RPL network with: a **root**, a **router (6LR)**, and a **leaf (6LN)** as shown in the figure. The **4th board** runs `sensniff` to capture traffic.

![Network topology](topo_tp2.png)

To **force a topology**, you may pin a desired parent on nodes so that DIOs from other nodes are ignored. In `project-conf.h`:
```c
#define RPL_CONF_CONN_TO "link-local IPv6 address of desired parent"
```

Implement the same UDP connection in previous tasks between **6LR (sender)** and **6LN (receiver)** but using global IP addresses of the RPL DODAG.

> Optional CLI: You can add the **shell** module in your `Makefile` to interact over the serial port (see the Contiki Cheat Sheet on Moodle for common commands).

**Work to do:**
1. Choose a **radio channel** (11–26) and **PAN ID** different from other groups to avoid interference.
2. Configure the stack: **CSMA (L2)**, **IPv6 (L3)**, and **RPL_LITE** as the routing protocol.
3. Complete `root.c` to configure one board as the **DODAG root**. Set an IPv6 **prefix** in the `fd00::/8` range (choose a prefix different from other groups).
4. Choose the **parent** for your 6LN and 6LR based on your physical setup.
5. Complete `6lr.c` (sender) and `6ln.c` (receiver) to establish UDP communication. **Start communication only after RPL has converged** (use routing driver APIs to check convergence).
7. Using Wireshark/Tshark, **trace the UDP communication**. What **path** do packets follow? What **routing type** is used?

---

## B.2) RPL Storing Mode

Modify your configuration to run RPL in **Storing** mode and re-run the UDP communication **after convergence**.
- Capture a new UDP trace.
- What path do the messages take in this configuration? Compare with the _Non-storing_ mode from the previous exercise (B.1).  
- What are the **advantages and disadvantages** of each mode?

---
