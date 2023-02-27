const hexToDecimal = hex => parseInt(hex, 16);

function hex2bin(hex){
    return (parseInt(hex, 16).toString(2)).padStart(8, '0');
}

export {hexToDecimal, hex2bin}