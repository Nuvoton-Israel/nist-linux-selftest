# NIST-800-193: linux flow

Nuvoton implemented a complete NIST-800-193 solution for NPCM7XX.
The solution includes a NIST-800-193 BootBlock, enhanced u-boot and linux procedure.
The BootBlock is provided by Nuvoton.
For u-boot Nuvoton provides reference code that should be integrated.
For linux Nuvoton provides this tool.

This tool implements a self-test procedure that reports it state accordingly.
Note that the function ‘**_int self_test(void)_**’ need to be customized.

For further info contact BMC_Marketing@Nuvoton.com
