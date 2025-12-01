# Extending Contiki-NG 6LoWPAN with 6LFF (Fragment Forwarding)

In this TP, you will extend the original Contiki-NG 6LoWPAN adaptation layer to support [**6LoWPAN Fragment Forwarding (6LFF)**](https://datatracker.ietf.org/doc/html/rfc8930) in RPL storing mode.

Before starting the implementation, you need to take a look at the [packet buffers used in the Contiki-NG network stack](https://docs.contiki-ng.org/en/master/doc/programming/Packet-buffers.html) and [6LoWPAN in Contiki-NG](https://docs.contiki-ng.org/en/master/_api/group__sicslowpan.html).

---

## 1. Learning Objectives

By the end of this TP, students should be able to:

1. Understand and implement 6LFF in Contiki-NG.
2. Compare the performance of 6LoWPAN with and without 6LFF.

---

## 2. Submission

Students should submit on Moodle an archive file (format: `.tgz`) that includes:

- The new version of `os/net/ipv6/sicslowpan.c` after modifications, in addition to any other modified files.
- A report in PDF format that details all introduced modifications (with the corresponding code) and responses to all questions, including the names of the group.
  - **Modifications without explanation will not be graded.**
- A folder containing the source files used to run a scenario that evaluates the new 6LFF feature, including the `Makefile` and `project-conf.h`.
- A Wireshark/Tshark trace in PCAP format of exchanged messages with and without 6LFF.

**Submission Deadline: Sunday 11, January 2026 - 20:00**

---

## 3. Step-by-Step Implementation Guide

### Step 0 – Initialization

- Add **LOG messages** in any section or function you add to explain what is going on; this may help you track the execution steps.
- We will work with **RPL storing mode** for this implementation. Non-storing mode implementation will be considered as a bonus.

---

### Step 1 – Add a Configuration Flag to Enable/Disable 6LFF (1 mark)

You should make 6LFF optional at compile time.

1. Introduce a configuration macro, e.g.:
   - A `SICSLOWPAN_CONF_6LFF` flag that can be defined as `1` in `project-conf.h` when you want 6LFF enabled.
2. Wrap all 6LFF-specific code with `#if` / `#endif` so that:
   - When the flag is **off**, the code behaves exactly like the original implementation.
   - When the flag is **on**, the 6LFF logic is compiled in.

---

### Step 2 – Explore the Existing Fragmentation Code

1. Locate the **fragment reassembly context structure** used by 6LoWPAN and check its members.
2. Locate the functions that:
   - **Create** a new reassembly context for a FRAG1.
   - **Add** a FRAGN to an existing context.
   - **Clear** or **expire** contexts.
3. Locate the 6LoWPAN **input function** where incoming frames are processed and:
   - Fragment headers (FRAG1/FRAGN) are parsed,
   - Fragments are reassembled,
   - Packets are finally delivered to the IPv6 stack.

---

### Step 3 – Extend the Reassembly Context with 6LFF State (1 mark)

You need extra state per fragmented datagram to support 6LFF:

1. Add a **boolean forwarding flag** to the reassembly context to indicate if the context uses 6LFF or normal reassembly.
2. Add a **field to store the MAC-layer next hop**.

---

### Step 4 – Initialize 6LFF State on New FRAG1 Contexts (1 mark)

When a new FRAG1 is received and a new reassembly context is created, make sure the new fields (forwarding flag and next hop) are initialized.

---

### Step 5 – Provide a Function to Look Up an Existing Context (2 marks)

Later fragments (FRAGN) need to **find the context** associated with a given datagram.

1. Implement a helper function `find_context(...)` that:
   - Takes a fragment **tag** and a **link-layer sender address**.
   - Scans the array of fragment contexts.
   - Returns the index (or an indication) of the matching context, if any.
2. The match should be based on:
   - The tag value.
   - The sender link-layer address stored in the context.
   - The context being currently “in use” (not empty or expired).

---

### Step 6 – Add a Helper to Forward the Current Fragment “As Is” (1 mark)

For 6LFF, once the node decides that a datagram should be forwarded:

1. Implement a helper function `forward_frag_as_is(...)` that directly sends the current `packetbuf` content to the given next hop.
2. This helper must **not** touch the 6LoWPAN or IP headers. It only changes the MAC receiver.

---

### Step 7 – Decide When to Apply 6LFF (Classification Rule) (1 mark)

1. After 6LoWPAN decompresses the IPv6 header for the first fragment (FRAG1), inspect the **destination IPv6 address**.
2. 6LFF should not be applied to every fragmented packet. Use 6LFF **only** when the packet destination has a valid, non–link-local and non–multicast IPv6 address and the current node is not the destination.
3. Define a boolean variable `use_6lff` set to `1` when 6LFF is applicable.

---

### Step 8 – Compute the Next Hop for 6LFF (2 marks)

1. Implement a helper function `find_nexthop_6LFF(...)` that:
   - Finds the link-layer (MAC) address of the next hop.
   - If the neighbor is missing from the **neighbor table**, log a warning and **drop the datagram** (clear context / do not forward).
   - Returns the link-layer address of the next hop or `NULL`.

---

### Step 9 – Mark the Context as "Forward" and Forward FRAG1 (2 marks)

If `use_6lff` is true for a FRAG1 and once you have the next hop MAC address for a 6LFF FRAG1:

1. In the reassembly context associated with the FRAG1:
   - Set the **forward flag**.
   - Store the **next hop MAC address**.
2. Call your `forward_frag_as_is(...)` helper to send the FRAG1 to the next hop at the MAC layer.
3. **Return** from the input function at this point for FRAG1, without reassembling or delivering the packet locally.

---

### Step 10 – Fast-Path for Subsequent Fragments (FRAGN) (2 marks)

For later fragments belonging to a 6LFF datagram (FRAGN):

1. After parsing the FRAGN headers and extracting the **tag** and **sender**:
   - Use your `find_context(...)` helper to locate the corresponding reassembly context.
2. If a context exists and is marked **forward-only**:
   - Call the `forward_frag_as_is(...)` helper with the stored **next hop**.
   - Return immediately from the input function.
3. If the context is not forward-only:
   - Fall back to the normal **reassembly** path for FRAGN.

---

### Step 11 – Testing and Experiments (5 marks)

To validate the implementation, you need to:

1. Build a **multi-hop RPL network** of one root and two children: `6LN -- root -- 6LN`. (1 mark)
2. Send a UDP packet between the children every 10 seconds with a large payload to force fragmentation. (1 mark)
3. Run **two experiments** in two modes:
   - **Baseline:** with 6LFF disabled.
   - **6LFF:** with the 6LFF enabled.
4. Observations to make:
   - Check logs to confirm that intermediate routers: (1 mark)
     - Reassemble and forward in the baseline.
     - Perform **fragment forwarding** (FRAG1 + FRAGN) when 6LFF is enabled.
5. Use a sniffer to capture the forwarded packets in both modes: (2 marks)
   - Track the sequence of the segments in both modes.
   - Track the time when the UDP packet is delivered to the destination.

---

## 4. Questions

1. Why is 6LFF especially important in **deep multi-hop** topologies? (1 mark)
2. What are the trade-offs between reassembling at every hop and forwarding fragments without reassembly? (1 mark)

---

## 5. BONUS (5 marks)

1. Try to extend the 6LFF implementation to non-storing mode (RPL-Lite).
   - You can use `#if RPL_WITH_NON_STORING` and `#if RPL_WITH_STORING` to check the running RPL mode.
2. Handle the root case, which has to reassemble the message always (normal behavior):
   - Root is the destination: assemble the message and send to upper layers.
   - Root is an intermediate node: reassemble the complete IPv6 message → send to upper layers → insert SRH → send to 6LoWPAN again for fragmentation.
   - You can use the `rpl_dag_root_is_root()` function to check whether a node is a root or not.
3. Modify `find_nexthop_6LFF(...)` to handle the non-storing mode in addition to the stroing mode.