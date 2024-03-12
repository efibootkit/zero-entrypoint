# Overview
This is a POC of module manipulation, it is manipulating the loaded module list within the PEB loader data table then swaps the Flink and Blink pointers with the BaseAddress and Entrypoint fields of LDR_MODULE structure, the memory is zeroed out which clears these fields.
The EntryPoint / BaseAddress of the module is nulled and not accessible by any debugger.
