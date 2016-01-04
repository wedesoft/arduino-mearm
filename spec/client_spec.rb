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
    expect(client).to receive(:write_serial).with('1 2 3 4c')
    client.target 1, 2, 3, 4
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
  it 'should query configuration' do
    expect(client).to receive(:write_serial).with('c')
    expect(client).to receive(:read_serial).and_return "1 2 3 4\r\n"
    expect(client.pos).to eq [1, 2, 3, 4]
  end
  it 'should read the lower limits' do
    expect(client).to receive(:write_serial).with('l')
    expect(client).to receive(:read_serial).and_return "-10 -20 -30 -45\r\n"
    expect(client.lower).to eq [-10, -20, -30, -45]
  end
  it 'should read the upper limits' do
    expect(client).to receive(:write_serial).with('u')
    expect(client).to receive(:read_serial).and_return "10 20 30 45\r\n"
    expect(client.upper).to eq [10, 20, 30, 45]
  end
end
