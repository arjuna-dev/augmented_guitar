export default class MIDIccMessage {
  constructor(channel, message_type, cc, value) {
    this.channel = channel;
    this.message_type = message_type;
    this.cc = cc;
    this.value = value;
  }
}
