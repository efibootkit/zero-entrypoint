# Overview
This is a POC of module manipulation, it is manipulating the loaded module list within the PEB loader data table then swaps the Flink and Blink pointers with the BaseAddress and Entrypoint fields of LDR_MODULE structure, the memory is zeroed out which clears these fields.
The EntryPoint / BaseAddress of the module is nulled and not accessible by any debugger.

Entry Point searched from its base address:
![Entry Point searched from its base address](https://cdn.discordapp.com/attachments/1142220291859292411/1217076952150708345/Capture_decran_2024-03-12_124900.png?ex=6602b63d&is=65f0413d&hm=1f51cf2c119064b68ac3a59fdda0c4591a20cebb5348ee6aa077da676cc819e8&)
