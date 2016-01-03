require_relative '../mearmwidget'

describe MeArmWidget do
  before :all do
    @app = Qt::Application.new []
  end
  let :client do
    double 'Client'
  end
  let :widget do
    expect(client).to receive(:pos).and_return([1, 2, 3, 4])
    expect(client).to receive(:lower).and_return([-10, -20, -30, -40])
    expect(client).to receive(:upper).and_return([+10, +20, +30, +40])
    MeArmWidget.new client
  end
  context 'when moving the base spin box' do
    it 'should move the base' do
      expect(client).to receive(:ready?).and_return true
      expect(client).to receive(:target).with(10, 2, 3, 4)
      widget.ui.baseSpin.setValue 10
    end
    it 'should start polling if robot not ready' do
      expect(client).to receive(:ready?).and_return false
      expect(widget).to receive(:defer)
      widget.ui.baseSpin.setValue 10
    end
  end
  context 'with pending updates' do
    before :each do
      widget.defer
    end
    it 'should process them when ready' do
      expect(client).to receive(:ready?).and_return true
      expect(client).to receive(:target)
      widget.pending
    end
    it 'should defer them if robot is not ready' do
      widget.defer
      expect(client).to receive(:ready?).and_return false
      expect(widget).to receive(:defer)
      widget.pending
    end
  end
  it 'should use values from the shoulder spin box' do
    expect(client).to receive(:ready?).and_return true
    expect(client).to receive(:target).with(1, 10, 3, 4)
    widget.ui.shoulderSpin.setValue 10
  end
  it 'should use values from the elbow spin box' do
    expect(client).to receive(:ready?).and_return true
    expect(client).to receive(:target).with(1, 2, 10, 4)
    widget.ui.elbowSpin.setValue 10
  end
  it 'should use values from the gripper spin box' do
    expect(client).to receive(:ready?).and_return true
    expect(client).to receive(:target).with(1, 2, 3, 10)
    widget.ui.gripperSpin.setValue 10
  end
  it 'should set the lower limits for the spin boxes' do
    expect(widget.ui.baseSpin.minimum    ).to eq -10
    expect(widget.ui.shoulderSpin.minimum).to eq -20
    expect(widget.ui.elbowSpin.minimum   ).to eq -30
    expect(widget.ui.gripperSpin.minimum ).to eq -40
  end
  it 'should set the upper limits for the spin boxes' do
    expect(widget.ui.baseSpin.maximum    ).to eq +10
    expect(widget.ui.shoulderSpin.maximum).to eq +20
    expect(widget.ui.elbowSpin.maximum   ).to eq +30
    expect(widget.ui.gripperSpin.maximum ).to eq +40
  end
end
