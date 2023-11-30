import { Sequence, TicksClass } from 'tone';

class ChordSequence extends Sequence {
    constructor(...args) {
        super(...args);
    }
    _rescheduleSequence(sequence, subdivision, startOffset) {
        sequence.forEach((value, index) => {
            const eventOffset = index * (subdivision) + startOffset;
            if (Array.isArray(value)) {
                if (value.length === 1 && Array.isArray(value[0])){
                    const startTime = new TicksClass(this.context, eventOffset, "i").toSeconds();
                    this._part.add(startTime, value.flat());
                } else {
                    this._rescheduleSequence(value, subdivision / value.length, eventOffset);
                }
            }
            else {
                const startTime = new TicksClass(this.context, eventOffset, "i").toSeconds();
                this._part.add(startTime, value);
            }
        });
    }
}

export default ChordSequence;