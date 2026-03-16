#!/usr/bin/env python3

import argparse
import logging

from uimg_header import create_uimg_header, UImg


def create_uimg(filename, lma, entry_point, name):
    output = bytearray()

    with open(filename, "rb") as fil:
        payload = fil.read()
        imgtype = UImg.image_type_kernel if entry_point > 0 else UImg.image_type_firmware

        logger.info("{} uimg LMA: 0x{:08x}, entry: 0x{:08x})".format(name, lma, entry_point))
        header, payload = create_uimg_header(payload, loadaddr=lma, entryaddr=entry_point, name=name, type=imgtype)
        output = header + payload

    return output

if __name__ == "__main__":
    parser = argparse.ArgumentParser("Create concatenated uimg from bin files. All addresses in hex (e.g. 0x70000000)")

    parser.add_argument("--a7", dest="a7_file", help="Input a7 bin file")
    parser.add_argument("--a7-lma", dest="a7_lma", help="Load address for a7 file", default="0x70080000")
    parser.add_argument("--a7-entry", dest="a7_entry", help="Entry address for a7 file")

    parser.add_argument("--fsbl", dest="fsbl_file", help="Input FSBL stm32 file")
    parser.add_argument("--fsbl-lma", dest="fsbl_lma", help="Load address for FSBL file", default="0x70000000")
    parser.add_argument("--fsbl-lma2", dest="fsbl_lma2", help="Second load address for FSBL file", default="0x70040000")

    parser.add_argument("--ssbl", dest="ssbl_file", help="Input SSBL bin file")
    parser.add_argument("--ssbl-lma", dest="ssbl_lma", help="Load address for SSBL file", default="0x70050000")

    parser.add_argument("out_file", help="Output image file")
    parser.add_argument("-v", dest="verbose", help="Verbose logging", action="store_true")
    args = parser.parse_args()

    logger = logging
    if args.verbose:
        logging.basicConfig(level=logging.DEBUG)

    with open(args.out_file, "wb") as out_file:
        if args.a7_file:
            output = create_uimg(args.a7_file, int(args.a7_lma, 16), int(args.a7_entry, 16), "App")
            out_file.write(output)

        if args.fsbl_file:
            output = create_uimg(args.fsbl_file, int(args.fsbl_lma, 16) , 0, "FSBL1")
            out_file.write(output)
            output = create_uimg(args.fsbl_file, int(args.fsbl_lma2, 16) , 0, "FSBL2")
            out_file.write(output)

        if args.ssbl_file:
            output = create_uimg(args.ssbl_file, int(args.ssbl_lma, 16), 0, "SSBL")
            out_file.write(output)






