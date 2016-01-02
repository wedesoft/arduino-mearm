require_relative '../client'

describe Client do
  before :each do
    port = double 'SerialPort'
    expect(port).to receive(:read_timeout=).with(2000)
    SerialPort.stub(:new) { port }
  end
  let :client do
    Client.new 'device', 1234
  end
  it 'should inform about time required' do
    expect(client).to receive(:write_serial).with('30t')
    expect(client).to receive(:read_serial).and_return "1.5\r\n"
    expect(client.time_required(30)).to eq 1.5
  end
  it 'should set drive targets' do
    expect(client).to receive(:write_serial).with('30c')
    client.target 30
  end
  it 'should inform about robot being ready' do
    expect(client).to receive(:write_serial).with('r')
    expect(client).to receive(:read_serial).and_return "1\r\n"
    expect(client.ready?).to be_true
  end
  it 'should inform about robot being busy' do
    expect(client).to receive(:write_serial).with('r')
    expect(client).to receive(:read_serial).and_return "0\r\n"
    expect(client.ready?).to be_false
  end
end
