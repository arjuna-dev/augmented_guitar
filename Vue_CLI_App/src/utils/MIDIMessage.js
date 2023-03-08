export default class MIDIMessage {
  constructor(channel, message_type, note, velocity) {
    this.channel = channel;
    this.message_type = message_type;
    this.note = note;
    this.velocity = velocity;
  }
}
